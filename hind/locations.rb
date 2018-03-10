# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    locations2.rb                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ivankozlov <ivankozlov@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/09 18:57:52 by ivankozlov        #+#    #+#              #
#    Updated: 2018/03/09 19:52:15 by ivankozlov       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

require "oauth2"
require "json"

def get_online_users(token)
	users = {}
	page = 0
	begin
		page += 1
		begin
			url = "v2/campus/7/locations?filter[active]=true&page=#{page}" 
			resp = token.get(url)
			resp.parsed.each do |entry|
				users[entry["user"]["login"]] = entry["host"]
			end
		rescue OAuth2::Error
			sleep(3)
			puts "API is not responding"
			retry
		end
	end while resp.headers["link"] && resp.headers["link"]["next"] 
	return users
end

def get_token()
	file = File.read('settings.json');
	json = JSON.parse(file)
	client = OAuth2::Client.new(json["uid"], json["secret"] , site: json["api_url"])
	return client.client_credentials.get_token
end

def get_input()
	users = {}
	ARGV.each do |a|
		File.open(a, "r") do |f|
			f.each_line do |line|
				users[line.delete("\n")] = ""
			end
		end
		rescue Errno::ENOENT, TypeError
			puts "File #{a} not found\n"
	end
	return users
end

token = get_token()
online_users = get_online_users(token)
users = get_input()

users.each do |login, value|
	begin
		response = token.get("/v2/users?filter[login]=#{login}")
		if response.parsed == []
			puts "#{login} - does not exist"
			users.delete(login)
		end
	rescue OAuth2::Error
		sleep(3)
		puts "API is not responding"
		retry
	end
end

users.each do |login, value|
	if (online_users.key?(login))
		puts "#{login} - #{online_users[login]}"
	else
		puts "#{login} - offline"
	end
end
