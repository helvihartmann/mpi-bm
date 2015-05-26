#!/usr/bin/env ruby

#13.05.2015 evolved from commStat from Gvozden Neskovic. reads in two files ibdmp.log (derived from dump_fts) and ibnet.log (derived from ibnetdiscover -lp) to modify existing routing forwarding tables such that they are optimal used for all to all linear shift communication pattern.
#Create output and write it into file load this file into subnetmanager opensm via: sudo /usr/sbin/opensm -f stdout -R file -U myTable.txt
#designed for a fattree network with 4 connections from each leaf to core switch

require 'pp'

def getdirectnodeswitchconnections(nodes, ib_phy, switchtranslatingtable)
    nodeswitchconnection = {}
    nodes.each do |node| # nodes << [node, guid, lid]
        
        nodeguid = node[1] #sender node guid I got from dump_fts
        connectedswitchguid = ib_phy[nodeguid][1][:dguid] #get guid of switch snode is connected to
        #translateswitch guid into readable name
        connectedswitch = switchtranslatingtable[connectedswitchguid]
        inp = ib_phy[nodeguid][1][:dport] #inport of snode
        
        nodeswitchconnection[node[0]] = {} #node[nodename, nodeguid, nodelid]
        nodeswitchconnection[node[0]][node[2]] = {}
        nodeswitchconnection[node[0]][node[2]] = {:switchguid => connectedswitchguid, :switch => connectedswitch, :port => inp}
    end
    return nodeswitchconnection
end


def getportsfromswitchtonodes(leafswitchtranslatingtable, nodeswitchconnection, leafswitchtocoreswitchportstable, coreswitchtranslatingtable, nodes, coreswitchportstable, nmbr_links)
    switchtonodes = {}

    leafswitchtranslatingtable.each do |switchguid, switch|
        l = 0
        c = 0
        connectedswitch = 0
        nodes.each do |node|
            nodename = node[0]
            #rack = nodename.gsub()
            nodeguid = node[1]
            nodelid = node[2]
            next if nodeswitchconnection[nodename][nodelid].nil?# why are some not in list?????
            if nodeswitchconnection[nodename][nodelid][switch].nil?#then no connection from node to switch exists
                if nodeswitchconnection[nodename][nodelid][:switchguid] != connectedswitch
                        l = 0
                end
                connectedswitch = nodeswitchconnection[nodename][nodelid][:switchguid]
                next if leafswitchtocoreswitchportstable[switchguid][switch].nil?
                #next if "#{nodename}" == "lqcd-login"
                switchtonodes["#{nodename} #{switch}"] = {}
                port = leafswitchtocoreswitchportstable[switchguid][switch]
                port_tmp = port[l]
                l = (l +1)%nmbr_links
                switchtonodes["#{nodename} #{switch}"] = port_tmp
            else
                coreswitchtranslatingtable.each do |guid, name|
                    switchtonodes["#{nodename} #{name}"] = {}
                    port = coreswitchportstable[switchguid][switch]
                    port_tmp = port[c]
                    c = (c + 1)%nmbr_links
                    switchtonodes["#{nodename} #{name}"] = port_tmp
                end
            end
        end
    end
    return switchtonodes
end

#!!!!!!!!!!!!!!!!!!!!------------------------GATHER INFO--------------------!!!!!!!!!!!!!!!!!!

nmbr_links = 4;
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
#nodelist
nodes = nodes.sort.uniq
sw_guidmap = sw_guidmap.uniq
nodelist = nodelist.sort_by{|lid, name| name}
nodelist = Hash[nodelist]
#------------------------Get Information----------------------
#about Connections-------------------
pm = `cat ibnet.log`.split("\n")
ib_phy = {}
switchtranslatingtable = {}
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
            node = nodelist[dlid2]
            if nodelist[dlid2].nil? or nodelist[slid2].nil?
            else
                switch = switchlist[dlid2]
                coreswitchtranslatingtable[sguid] = "#{switch}"
            end
            next if nodelist[dlid2].nil?
            
            node = node.gsub("r",'').gsub("n01",'')
            switchtranslatingtable[sguid] = "#{node}"
        end
    end
end

coreswitchtranslatingtable = coreswitchtranslatingtable.delete_if{ |k, v| v == "" }
i = 0
coreswitchtranslatingtable.each do |guid, name|
    coreswitchtranslatingtable[guid] = "coreswitch#{i}"
    i += 1
    leafswitchtranslatingtable = switchtranslatingtable.delete_if{ |k, v| k == guid }
end
leafswitchtranslatingtable = leafswitchtranslatingtable.sort_by{|sguid, nodename| nodename}
leafswitchtranslatingtable = Hash[leafswitchtranslatingtable]
switchtranslatingtable = leafswitchtranslatingtable.merge(coreswitchtranslatingtable)
# about switch ports------------------------------

                    
coreswitchportstable = []
portsleaf = []
portscore = []
leafswitchtocoreswitchportstable = {}
coreswitchportstable = {}
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
# these nodes stay put
nodeswitchconnection = getdirectnodeswitchconnections(nodes, ib_phy, switchtranslatingtable)

# these nodes are reshuffled
switchtonodes = getportsfromswitchtonodes(leafswitchtranslatingtable, nodeswitchconnection, leafswitchtocoreswitchportstable, coreswitchtranslatingtable, nodes, coreswitchportstable, nmbr_links)

#------------------------PRINT--------------------
switchtranslatingtable.each do |switchguid, value|
    switchname = leafswitchtranslatingtable[switchguid]
    header = "Unicast lids [0x0-0xaa] of switch guid"
    header += " #{switchguid} (#{switchname})"
    subheader1 = "  Lid  Out   Destination"
    subheader2 = "       Port     Info "
    puts header
    puts subheader1
    puts subheader2
    i = 0
    nodelist.each do |lid, node|
        next if nodeswitchconnection[node][lid].nil?
                    
        if coreswitchtranslatingtable[switchguid] != nil
            destswitchguid = nodeswitchconnection[node][lid][:switchguid]
            destswitchname = switchtranslatingtable[destswitchguid]
            next if destswitchname.nil?
                    #pp destswitchguid
            port = coreswitchportstable[destswitchguid][destswitchname]
            port_tmp = port[i]
            forwardlist = "0x#{lid} %03d" %port_tmp
            i = (i + 1)%nmbr_links
        else
            if (nodeswitchconnection[node][lid][:switchguid] == switchguid) then
                #port = switches_lid[switchguid][lid]
                port = nodeswitchconnection[node][lid][:port]
                forwardlist = "0x#{lid} %03d" %port
            else

                port = switchtonodes["#{node} #{switchname}"]
                next if port.nil?
                forwardlist = "0x#{lid} %03d" %port
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
                    #pp nodelist
                    #puts ''
                    #sw_guidmap
                    #puts ''
                    #ib_phy
                    #puts ''
                    #pp leafswitchtranslatingtable
                    #puts ''
                    #pp coreswitchtranslatingtable
                    #puts ''
                    #switchtranslatingtable
                    #puts ''
                    #leafswitchtocoreswitchportstable
                    #puts ''
                    #pp coreswitchportstable
                    #puts ''
                    #pp nodeswitchconnection
                    #puts ''
                    #pp switchtonodes
