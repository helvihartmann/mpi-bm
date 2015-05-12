#!/usr/bin/env ruby

require 'pp'
require 'tempfile'
require 'yaml'

#nodes
node_lid = {}
node_pguid = {}
node_name = {}

ibtf = Tempfile.new('ib_inventory.log')
ibtf.close

`osmtest -f c -i #{ibtf.path}`

ibtf.open

ibinv = ibtf.read.split("\n")
lid = 0
mode = :unknown
done = false

d = {}

ibinv.each do |ibl|
  mode,done = case ibl
    when /DEFINE_NODE/ then [:node, false]
    when /DEFINE_PATH/ then [:path, false]
    when /DEFINE_LINK/ then [:link, false]
    when /END/         then [mode,  true]
    else [mode,done]
  end

  if !done && mode == :node
    if md = ibl.match(/^lid\s+(.+)$/)
      d[:lid] = md[1]
    elsif md = ibl.match(/^port_guid\s+(.{18})$/)
      d[:pguid] = md[1]
    elsif md = ibl.match(/node_desc\s+(.+)$/)
      d[:name] = md[1]
    end
  end

  if done && mode == :node
    node_lid[d[:lid]] = { :name => d[:name], :lid => d[:lid], :pguid => d[:pguid] }
    node_pguid[d[:pguid]] = d[:lid]
    node_name[d[:name].gsub(".lqcd.gsi.de","")] = d[:lid]
  end

end

ibtf.unlink

#pp node_name

# routes
routes = {}
if File.exists?('routeHash.yaml')
  routes = YAML.load_file('routeHash.yaml')
else
  node_pguid.each do |s_pguid, s_lid|
    node_pguid.each do |d_pguid, d_lid|
      tr = `ibtracert -n -G #{s_pguid} #{d_pguid}`.split("\n")
      r = []
      sg = s_pguid
      tr.each do |l|
        if md = l.match(/^\[(.+)\].+\{(.{18})\}\[(.+)\]$/)
          r << { :s_pguid => sg, :s_port => md[1], :d_pguid => md[2], :d_port => md[3] }
          sg = md[2]
        end
      end
      routes[s_pguid] = {} if routes[s_pguid].nil?
      routes[s_pguid][d_pguid] = r
    end
  end
  File.open('routeHash.yaml','w') { |f| f.write(routes.to_yaml) }
end

#pp routes


# calbes
links = {}
routes.each do |s_pguid, dest|
  dest.each do |d_pguid, lnks |
    lnks.each do |lnk|
      links["#{lnk[:s_pguid]}:#{lnk[:s_port]}"] = "#{lnk[:d_pguid]}:#{lnk[:d_port]}"
    end
  end
end

data = `cat 60vs60.log`.split("\n").map(&:strip).map(&:split)

passes = {}
pass = 0
data.each do |l|

  if l.length == 1 and l[0].to_i >= 0 then
    pass = l[0].to_i
    passes[pass] = { :same => 0, :cross => 0, :cbw => 0.0, :switch => {} }
    puts ""
  end

  if l.length == 5 then

    md = /.+r(\d\d).+r(\d\d).+/.match(l[0])
    r1 = md[1]
    r2 = md[2]

    # pp "#{r1} -> #{r2}"

    passes[pass][:same] +=1  if r1 == r2
    passes[pass][:cross] +=1 if r1 != r2

    passes[pass][:cbw] += l[4].to_f

    passes[pass][:switch][r1] = 0.0 if passes[pass][:switch][r1].nil?
    passes[pass][:switch][r1] += l[4].to_f

    # link histogram
    if passes[pass][:lnk_hist].nil?
      passes[pass][:lnk_hist] = {}
      links.each do |lnk_s, lnk_e|
        passes[pass][:lnk_hist]["#{lnk_s}--#{lnk_e}"] = 0
      end
    end
    md = /(.{11})(.{11})/.match(l[0])
    node1_name = md[1]
    node2_name = md[2]
    node1_pguid = node_lid[node_name[node1_name]][:pguid]
    node2_pguid = node_lid[node_name[node2_name]][:pguid]
    route_data = routes[node1_pguid][node2_pguid]

    route_data.each do |rd|
      link_name = "#{rd[:s_pguid ]}:#{rd[:s_port]}--#{rd[:d_pguid]}:#{rd[:d_port]}"
      #passes[pass][:lnk_hist][link_name] = 0 if passes[pass][:lnk_hist][link_name].nil?
      passes[pass][:lnk_hist][link_name] += 1

      #print bad ones
      if passes[pass][:lnk_hist][link_name] > 1
        puts "#{pass}: #{node1_name} -> #{node2_name} - link #{link_name} used #{passes[pass][:lnk_hist][link_name]} times"
      end
    end
  end
end

#pp passes
