# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    locations.rb                                       :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ivankozlov <ivankozlov@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/09 16:12:03 by ivankozlov        #+#    #+#              #
#    Updated: 2018/03/09 16:50:40 by ivankozlov       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

require "oauth2"
require "json"

file = File.read('settings.json');
json = JSON.parse(file)
client = OAuth2::Client.new(json["uid"], json["secret"] , site: json["api_url"])
token = client.client_credentials.get_token

users = {}
ARGV.each do |a|
	File.open(a, "r") do |f|
		f.each_line do |line|
			users[line.delete("\n")] = "offline"
		end
	end
	rescue Errno::ENOENT, TypeError
		puts "File #{a} not found\n"
end

users.each do |login, status|
	response = token.get("/v2/users/#{login}")
	if response.status == 200 && response.parsed["location"]
		users[login] = response.parsed["location"]
	end
	rescue OAuth2::Error => e
		users[login] = "Does not exist"
end

# resp = nil
# count = 0
# page = 0
# begin
# 	page += 1
# 	url = "v2/campus/7/locations?filter=active&page=#{page}" 
# 	resp = token.get(url)
# 	resp.parsed.each do |entry|
# 		if (users[entry["user"]["login"]] == "offline")
# 			count += 1
# 			users[entry["user"]["login"]] = entry["host"]
# 		end
# 	end
# end while resp.headers["link"] && resp.headers["link"]["next"] && count < users.count

users.each do |login, status|
	puts "#{login} - #{status}"
end
