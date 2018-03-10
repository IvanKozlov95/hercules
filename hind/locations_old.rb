# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    locations.rb                                       :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ivankozlov <ivankozlov@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/09 16:12:03 by ivankozlov        #+#    #+#              #
#    Updated: 2018/03/09 19:09:01 by ivankozlov       ###   ########.fr        #
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
			users[line.delete("\n")] = "unknown"
		end
	end
	rescue Errno::ENOENT, TypeError
		puts "File #{a} not found\n"
end

max_errors = 10
error_count = 0
while users.count > 0 do
	users.each do |login, status|
		response = token.get("/v2/users?filter[login]=#{login}")
		if response.parsed == []
			puts "#{login} - does not exist"
			users.delete(login)
		else
			response = token.get("/v2/users/#{login}")
			if response.status == 200 && response.parsed["location"]
				puts "#{login} - #{response.parsed["location"]}"
				users.delete(login)
			else
				puts "#{login} - offline"
				users.delete(login)
			end
		end
		rescue OAuth2::Error
			error_count += 1
			if (error_count >= max_errors)
				puts "42 API is probably dead. I'm tired of trying.\n"
				exit -1
			end
			break ;
	end
end
