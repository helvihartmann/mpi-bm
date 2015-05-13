#!/usr/bin/env ruby

require 'pp'

rt = `cat ibdmp.log`.split("\n")

switches = {}
switches_lid = {}
nodes = []
sw_guid = 0
sw_guidmap = []
usednodes = ['r05n01', 'r05n02', 'r05n03', 'r05n04', 'r06n01', 'r06n02', 'r06n03', 'r06n04', 'r07n01', 'r07n02', 'r07n03', 'r07n04', 'r08n01', 'r08n02', 'r08n03', 'r08n04']
rt.each do |rl|
  if md = rl.match(/^Unicast lids.*of switch.*guid\s(\S{18}).*$/) then
    sw_guid = md[1]
    switches[sw_guid], switches_lid[sw_guid] = {}, {}
  end
  
  sw_guidmap.push(sw_guid)

  if md = rl.match(/^0x(\S{4})\s(\S+)\s.*Channel Adapter portguid\ (\S{18}).*\'(\S+)\'\)$/) then
    #pp md
    lid = md[1].hex
    port = md[2].to_i
    guid = md[3]
    node = md[4].gsub(".lqcd.gsi.de",'').gsub("lcsc-",'')
    
    if usednodes.include? node then
        if mdn = node.match(/r(\d\d)n(\d\d)/)
          node = "r#{mdn[1]}n#{mdn[2]}"
        end
        nodes << [node, guid, lid]

        switches[guid] = node

        switches_lid[sw_guid][lid] = port
    end
  end

end

#my stuff
sw_guidmap = sw_guidmap.uniq
sw_guidmap.each_with_index do |item, index|
                   #puts index
end

nodes = nodes.uniq.sort

#pp sw_guidmap
#pp switches_lid
pp nodes

pm = `cat ibnet.log`.split("\n")

#-------------------------------------
                   #dguid_map = []
                   #sguid_map = []
                   #pm.each do |lnk|
                   #if (md = lnk.match(/^CA\s*(\S{1,3})\s*(\S{1,3})\s+(\S{18}).+\-\s*SW\s+(\d{1,3})\s+(\d{1,3})\s+(\S{18}).*$/)) or
                   #(md = lnk.match(/^SW\s*(\S{1,3})\s*(\S{1,3})\s+(\S{18}).+\-\s*SW\s+(\d{1,3})\s+(\d{1,3})\s+(\S{18}).*$/)) or
                   #(md = lnk.match(/^SW\s*(\S{1,3})\s*(\S{1,3})\s+(\S{18}).+\-\s*CA\s+(\d{1,3})\s+(\d{1,3})\s+(\S{18}).*$/))
                   #pp md
                   #sguid = md[3]
                   #dguid = md[6]
                   #sguid_map.push(sguid)

                   #end
                   #end

                   #sguid_map = sguid_map.uniq
                   #sguid_map.each_with_index do |sguid_item, sguid_index|
                   #puts sguid_item
                   #puts sguid_index
                   #end
#-------------------------------------

ib_phy = {}
                   
pm.each do |lnk|
  # pp lnk
  if (md = lnk.match(/^CA\s*(\S{1,3})\s*(\S{1,3})\s+(\S{18}).+\-\s*SW\s+(\d{1,3})\s+(\d{1,3})\s+(\S{18}).*$/)) or
     (md = lnk.match(/^SW\s*(\S{1,3})\s*(\S{1,3})\s+(\S{18}).+\-\s*SW\s+(\d{1,3})\s+(\d{1,3})\s+(\S{18}).*$/)) or
     (md = lnk.match(/^SW\s*(\S{1,3})\s*(\S{1,3})\s+(\S{18}).+\-\s*CA\s+(\d{1,3})\s+(\d{1,3})\s+(\S{18}).*$/))
                   #pp md
    slid = md[1].to_i
    sport = md[2].to_i
    sguid = md[3]
    dlid = md[4].to_i
    dport = md[5].to_i
    dguid = md[6]

    ib_phy[sguid] = {} if ib_phy[sguid].nil?
                   
    ib_phy[sguid][sport] = {} if ib_phy[sguid][sport].nil?

    ib_phy[sguid][sport] = {:dguid => dguid, :dport => dport}
  end
end
                   
# pp data
# pp passes
# pp rt
# pp switches
# pp switches_lid
# pp nodes
#pp ib_phy

#ib_phy.each { |a| print a, "\n" }
#pp ib_phy
csw_list = []
nodes.each do |snode|
  nodes.each do |enode|

    next if snode == enode

    elid = enode[2]

    # sw = ib_phy[snode[1]][1][:dguid]
    # isw_port = ib_phy[snode[1]][1][:dport]

    cdev = snode[1] #sender node guid I got from dump_fts
    cport = 1 #port of cables from node to switch always 1

    #pp snode
    #pp enode
                   
    path = "#{switches[snode[1]]}-#{cport}"

    csw = ib_phy[cdev][cport][:dguid] #get guid of switch snode is connected to
    inp = ib_phy[cdev][cport][:dport] #inport of snode
    outp = switches_lid[csw][elid] #from dump_fts switches_lid[sw_guid][lid] = port


   csw_list.push(csw)
   #pp inp
    loop do

      path += ("--#{inp}-switch#{csw}-#{outp}")


      nsw = ib_phy[csw][outp][:dguid]
      break if nsw.nil?
      ninp = ib_phy[csw][outp][:dport]
      break if ninp.nil?
      break if switches_lid[nsw].nil?
      noutp = switches_lid[nsw][elid]
      break if noutp.nil?

      inp,csw,outp = ninp,nsw,noutp
     end

    path += "--1-#{switches[enode[1]]}"
    puts path

  end
end
#csw_list = csw_list.uniq
#pp csw_list