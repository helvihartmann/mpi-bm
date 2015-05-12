#!/usr/bin/env ruby

require 'pp'

#------------------------GATHER INFO--------------------
rt = `cat ibdmp.log`.split("\n")

switches = {}
switches_lid = {}
nodes = []
sw_guid = 0
sw_guidmap = []
lidlist = []
nodelist = {}
usednodes = ['r04n06', 'r04n07', 'r04n08', 'r04n09', 'r05n01', 'r05n02', 'r05n03', 'r05n04', 'r06n01', 'r06n02', 'r06n03', 'r06n04', 'r07n01', 'r07n02', 'r07n03', 'r07n04', 'r08n01', 'r08n02', 'r08n03', 'r08n04', 'r09n01', 'r09n12']
rt.each do |rl|
   if md = rl.match(/^Unicast lids.*of switch.*guid\s(\S{18}).*$/) then
     sw_guid = md[1]
     switches[sw_guid], switches_lid[sw_guid] = {}, {}
   end
  
   if (md = rl.match(/^0x(\S{4})\s(\S+)\s.*Channel Adapter portguid\ (\S{18}).*\'(.+)\'\)$/)) 

    #or(md = rl.match(/^0x(\S{4})\s(\S+)\s.*Switch portguid\ (\S{18}).*\'(.+)\'\)$/)) this collects switch to switch forwarding tables, not sure if I need it
    #node = md[4].gsub(" mlx4","_0").gsub(" mlx5","_1")
    node = md[4].gsub(".lqcd.gsi.de",'').gsub("lcsc-",'')
    
    #if usednodes.include? node then
    lid = md[1]
    port = md[2].to_i
    guid = md[3]
                       
    if mdn = node.match(/r(\d\d)n(\d\d)/)
      node = "r#{mdn[1]}n#{mdn[2]}"
    end
    nodes << [node, guid, lid]
    nodelist [lid] = node
    switches[guid] = node
    switches_lid[sw_guid][lid] = port
    lidlist.push(lid)
    sw_guidmap << sw_guid


    #end
  end
end
        
                       
sw_guidmap = sw_guidmap.uniq
nodelist = nodelist.sort_by {|key, value| value}
nodelist_sorted = Hash[nodelist]
lidlist = lidlist.uniq


   
#------------------------Get Information about Connections-------------------
pm = `cat ibnet.log`.split("\n")
ib_phy = {}
switchtranslatingtable = {}
pm.each do |lnk|
    # pp lnk
   if (md = lnk.match(/^SW\s*(\S{1,3})\s*(\S{1,3})\s+(\S{18}).+\-\s*SW\s+(\d{1,3})\s+(\d{1,3})\s+(\S{18}).*$/))
   
   end
    if (md = lnk.match(/^CA\s*(\S{1,3})\s*(\S{1,3})\s+(\S{18}).+\-\s*SW\s+(\d{1,3})\s+(\d{1,3})\s+(\S{18}).*$/)) or
        (md = lnk.match(/^SW\s*(\S{1,3})\s*(\S{1,3})\s+(\S{18}).+\-\s*SW\s+(\d{1,3})\s+(\d{1,3})\s+(\S{18}).*$/)) or
        (md = lnk.match(/^SW\s*(\S{1,3})\s*(\S{1,3})\s+(\S{18}).+\-\s*CA\s+(\d{1,3})\s+(\d{1,3})\s+(\S{18}).*$/))
        #pp md
        slid = md[1]
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
            node = nodelist_sorted[dlid2]
            next if node.nil?
            node = node.gsub("r",'').gsub("n01",'')
            switchtranslatingtable[sguid] = "#{node}"
        end
    end
end
                     
# about leafswitche ports------------------------------

switchtranslatingtable = switchtranslatingtable.sort_by{|sguid, nodename| nodename}
switchtranslatingtable = Hash[switchtranslatingtable]
coreswitchportstable = []
ports = []
leafswitchtocoreswitchportstable_hash = {}
coreswitchportstable_hash = {}
destinationswitch_prev = "09"
switchtranslatingtable.each do |switchguid, switch|
    for swport in 1..36
    next if ib_phy[switchguid][swport].nil?
    switchname = switchtranslatingtable[switchguid]
    port = ib_phy[switchguid][swport][:dport]

    if sw_guidmap.include? ib_phy[switchguid][swport][:dguid]
                       ports << port
       end
    end
    
    leafswitchtocoreswitchportstable_hash[switchguid] = {}
    leafswitchtocoreswitchportstable_hash[switchguid][switchname] = ports
    ports = []
end
                     
# about core switch ports------------------------------
 for swport in 1..36
     switchguid = "0xf4521403005cc590"
     next if ib_phy[switchguid][swport].nil?
     switchname = switchtranslatingtable[switchguid]
     port = ib_phy[switchguid][swport][:dport]

     destinationswitchguid = ib_phy[switchguid][swport][:dguid]
     destinationswitch = switchtranslatingtable[destinationswitchguid]
     
     if destinationswitch_prev != destinationswitch
        ports = []
     end
     coreswitchportstable_hash[destinationswitchguid] = destinationswitch
     coreswitchportstable_hash[destinationswitchguid] = {}
     coreswitchportstable_hash[destinationswitchguid][destinationswitch] = ports
     destinationswitch_prev = destinationswitch
     ports << port
     #coreswitchportstable << [switchguid, switchname, port, destinationswitch]
 end

#------------------------REORDER--------------------
# these nodes stay put------------------------------
nodes = nodes.sort.uniq
nodeswitchconnection = {}
nodes.each do |node| # nodes << [node, guid, lid]

    nodeguid = node[1] #sender node guid I got from dump_fts
    connectedswitchguid = ib_phy[nodeguid][1][:dguid] #get guid of switch snode is connected to
    #translateswitch guid into readable name
    connectedswitch = switchtranslatingtable[connectedswitchguid]
    inp = ib_phy[nodeguid][1][:dport] #inport of snode

   nodeswitchconnection[node[0]] = {} #node[nodename, nodeguid, nodelid]
   nodeswitchconnection[node[0]][node[2]] = {}
   nodeswitchconnection[node[0]][node[2]][connectedswitch] = {}
   nodeswitchconnection[node[0]][node[2]][connectedswitch] = inp
end
                       


                     
# these nodes are reshuffled------------------------------
switchtonodes = {}
i = 0
switchtranslatingtable.each do |switchguid, switch|
    nodes.each do |node|
        nodename = node[0]
        nodeguid = node[1]
        nodelid = node[2]
        next if nodeswitchconnection[nodename][nodelid].nil?# why are some not in list?????
        if nodeswitchconnection[nodename][nodelid][switch].nil?#then no connection from node to switch exists
            switchtonodes["#{nodename} #{switch}"] = {}
            next if leafswitchtocoreswitchportstable_hash[switchguid][switch].nil?
            next if "#{nodename} #{switch}" == "lqcd-login 02"
             i = i%4
             port = leafswitchtocoreswitchportstable_hash[switchguid][switch]
             port_tmp = port[i]
             switchtonodes["#{nodename} #{switch}"] = port_tmp
             i = i + 1
        end
    end
end


#------------------------PRINT--------------------
switches_lid.each do |switchguid, value|
    header = "Unicast lids [0x0-0xaa] of switch guid"
    header += " #{switchguid}"
    subheader1 = "  Lid  Out   Destination"
    subheader2 = "       Port     Info "
                       #puts header
                       #puts subheader1
                       #puts subheader2
   switchname = switchtranslatingtable[switchguid]
   lidlist.each do |lid|
        node = nodelist_sorted[lid]
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
            forwardlist = "0x#{lid} ?"
        end
        
        forwardlist += " : (Channel Adapter portguid "
        forwardlist += "guid: "
        forwardlist += "#{node})"
                       #puts forwardlist
   end
end

puts 'sw_guidmap'
pp sw_guidmap
puts ' '
#puts 'nodes'
#pp nodes
#puts ' '
#puts 'nodeswithconenction'
#pp nodeswitchconnection
#puts ' '
puts 'switchtranslatingtable'
pp switchtranslatingtable
puts ' '
#puts 'switchtonodes'
#pp switchtonodes
#puts ' '
#puts 'leafswitchtocoreswitch'
#pp leafswitchtocoreswitchportstable_hash
#puts ' '
#puts 'coreswitchports'
#pp coreswitchportstable_hash
#puts ' '
#puts 'nodeswithconnection'
#pp nodeswitchconnection