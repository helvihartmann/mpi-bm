#!/usr/bin/env ruby

require 'pp'

#------------------------GATHER INFO--------------------
rt = `cat ibdmp.log`.split("\n")

nodes = []
sw_guid = 0
sw_guidmap = []
nodelist = {}
switchlist = {}
rt.each do |rl|
   if md = rl.match(/^Unicast lids.*of switch.*guid\s(\S{18}).*$/) then
       sw_guid = md[1]
   end
  
   if (md = rl.match(/^0x(\S{4})\s(\S+)\s.*Channel Adapter portguid\ (\S{18}).*\'(.+)\'\)$/)) or
        (md = rl.match(/^0x(\S{4})\s(\S+)\s.*Switch portguid\ (\S{18}).*\'(.+)\'\)$/))
    #node = md[4].gsub(" mlx4","_0").gsub(" mlx5","_1")
    node = md[4].gsub(".lqcd.gsi.de",'').gsub("lcsc-",'')
    lid = md[1]
    port = md[2].to_i
    guid = md[3]
                       
    if mdn = node.match(/r(\d\d)n(\d\d)/)
      node = "r#{mdn[1]}n#{mdn[2]}"
    end
    nodes << [node, guid, lid]
    nodelist [lid] = node
    sw_guidmap << sw_guid
  end
                     
  if (md = rl.match(/^0x(\S{4})\s(\S+)\s.*Switch portguid\ (\S{18}).*\'(.+)\'\)$/))
    lid = md[1]
    switch = md[4]
    guid = md[3]
    switchlist[lid] = switch
  end
end
#switchlist
#nodes
#nodelist
sw_guidmap = sw_guidmap.uniq
   
#------------------------Get Information about Connections-------------------
pm = `cat ibnet.log`.split("\n")
ib_phy = {}
leafswitchtranslatingtable = {}
coreswitchtranslatingtable = {}
pm.each do |lnk|
    if (md = lnk.match(/^CA\s*(\S{1,3})\s*(\S{1,3})\s+(\S{18}).+\-\s*SW\s+(\d{1,3})\s+(\d{1,3})\s+(\S{18}).*$/)) or
        (md = lnk.match(/^SW\s*(\S{1,3})\s*(\S{1,3})\s+(\S{18}).+\-\s*SW\s+(\d{1,3})\s+(\d{1,3})\s+(\S{18}).*$/)) or
        (md = lnk.match(/^SW\s*(\S{1,3})\s*(\S{1,3})\s+(\S{18}).+\-\s*CA\s+(\d{1,3})\s+(\d{1,3})\s+(\S{18}).*$/))
        slid = md[1].to_i
        slid2 = "00"
        slid2 += slid.to_s(16)
        sport = md[2].to_i
        sguid = md[3]
        dlid = md[4].to_i
        dlid2 = "00"
        dlid2 += dlid.to_s(16)
        dport = md[5].to_i
        dguid = md[6]

        ib_phy[sguid] = {} if ib_phy[sguid].nil?
        ib_phy[sguid][sport] = {} if ib_phy[sguid][sport].nil?
        ib_phy[sguid][sport] = {:dguid => dguid, :dport => dport}
          
        
        if sw_guidmap.include? sguid then
            if nodelist[dlid2].nil? or nodelist[slid2].nil?
                switch = switchlist[dlid2]
                coreswitchtranslatingtable[sguid] = "#{switch}"
            end
            next if nodelist[dlid2].nil?
            node = nodelist[dlid2]
            node = node.gsub("r",'').gsub("n01",'')
            leafswitchtranslatingtable[sguid] = "#{node}"
        end
    end
end
coreswitchtranslatingtable = coreswitchtranslatingtable.delete_if{ |k, v| v == "" }
i = 0
coreswitchtranslatingtable.each do |guid, name|
    coreswitchtranslatingtable[guid] = "coreswitch#{i}"
    i += 1
end
switchtranslatingtable = leafswitchtranslatingtable.merge(coreswitchtranslatingtable)

# about switch ports------------------------------

leafswitchtranslatingtable = leafswitchtranslatingtable.sort_by{|sguid, nodename| nodename}
leafswitchtranslatingtable = Hash[leafswitchtranslatingtable]
coreswitchportstable = []
portsleaf = []
portscore = []
leafswitchtocoreswitchportstable = {}
coreswitchportstable = {}
destinationswitch_prev = "09"
leafswitchtranslatingtable.each do |switchguid, switch|
    for swport in 1..36
    next if ib_phy[switchguid][swport].nil?
        switchname = leafswitchtranslatingtable[switchguid]
        port = ib_phy[switchguid][swport][:dport]
        next if port.nil?
        if sw_guidmap.include? ib_phy[switchguid][swport][:dguid]
            portsleaf << swport
            portscore << port
        end
    end
    leafswitchtocoreswitchportstable[switchguid] = {}
    leafswitchtocoreswitchportstable[switchguid][switchname] = portsleaf
                    
    coreswitchportstable[switchguid] = {}
    coreswitchportstable[switchguid][switchname] = portscore
    portscore = []
    portsleaf = []
end

#------------------------REORDER--------------------
# these nodes stay put------------------------------
nodes = nodes.sort.uniq
nodeswitchconnection = {}
nodes.each do |node| # nodes << [node, guid, lid]

    nodeguid = node[1] #sender node guid I got from dump_fts
    connectedswitchguid = ib_phy[nodeguid][1][:dguid] #get guid of switch snode is connected to
    #translateswitch guid into readable name
    connectedswitch = leafswitchtranslatingtable[connectedswitchguid]
    inp = ib_phy[nodeguid][1][:dport] #inport of snode

   nodeswitchconnection[node[0]] = {} #node[nodename, nodeguid, nodelid]
   nodeswitchconnection[node[0]][node[2]] = {}
   nodeswitchconnection[node[0]][node[2]][connectedswitch] = {}
   nodeswitchconnection[node[0]][node[2]][connectedswitch] = inp
end
                    
                     
# these nodes are reshuffled------------------------------
switchtonodes = {}
l = 0
c = 0
leafswitchtranslatingtable.each do |switchguid, switch|
    nodes.each do |node|
        nodename = node[0]
        nodeguid = node[1]
        nodelid = node[2]
        next if nodeswitchconnection[nodename][nodelid].nil?# why are some not in list?????
        if nodeswitchconnection[nodename][nodelid][switch].nil?#then no connection from node to switch exists
            next if leafswitchtocoreswitchportstable[switchguid][switch].nil?
            next if "#{nodename}" == "lqcd-login"
            switchtonodes["#{nodename} #{switch}"] = {}
            l = l%4
            port = leafswitchtocoreswitchportstable[switchguid][switch]
            port_tmp = port[l]
            l += 1
            switchtonodes["#{nodename} #{switch}"] = port_tmp
        else
            coreswitchtranslatingtable.each do |guid, name|
                    switchtonodes["#{nodename} #{name}"] = {}
                    c = c%4
                    port = coreswitchportstable[switchguid][switch]
                    port_tmp = port[c]
                    c += 1
                    switchtonodes["#{nodename} #{name}"] = port_tmp
            end
        end
    end
end


#------------------------PRINT--------------------
switchtranslatingtable.each do |switchguid, value|
    header = "Unicast lids [0x0-0xaa] of switch guid"
    header += " #{switchguid}"
    subheader1 = "  Lid  Out   Destination"
    subheader2 = "       Port     Info "
    puts header
    puts subheader1
    puts subheader2
    switchname = leafswitchtranslatingtable[switchguid]
    nodelist.each do |lid, node|
        next if nodeswitchconnection[node][lid].nil?
        if (nodeswitchconnection[node][lid][switchname] != nil) then
            #port = switches_lid[switchguid][lid]
            port = nodeswitchconnection[node][lid][switchname]
            if port < 10 then
                forwardlist = "0x#{lid} 00#{port}"
            else
                forwardlist = "0x#{lid} 0#{port}"
            end
        else
            port = switchtonodes["#{node} #{switchname}"]
            next if port.nil?
            if port < 10 then
                forwardlist = "0x#{lid} 00#{port}"
            else
                forwardlist = "0x#{lid} 0#{port}"
            end
        end
        
        forwardlist += " : (Channel Adapter portguid "
        forwardlist += "guid: "
        forwardlist += "#{node})"
        puts forwardlist
   end
end

                    #------------------ Print out existing Hashes and Arrays
                    #puts 'switch list'
                    #switchlist
                    #puts ''
                    #nodes
                    #puts ''
                    #nodelist
                    #puts ''
                    #sw_guidmap
                    #puts ''
                    #ib_phy
                    #puts ''
                    #leafswitchtranslatingtable
                    #puts ''
                    #coreswitchtranslatingtable
                    #puts ''
                    #switchtranslatingtable
                    #puts ''
                    #leafswitchtocoreswitchportstable
                    #puts ''
                    #coreswitchportstable
                    #puts ''
                    #nodeswitchconnection[node[0]]
                    #puts ''
                    #switchtonodes
