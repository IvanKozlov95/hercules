require "oauth2"

UID = "2b9a0883f878ec13e7556f83833d69d448e7868d5e53812b737d0989710d6149"
SECRET = "d2fe665865cc3805ef59b8e54cd6ee88c255a658aca3733340d8a1c9d88923a8"

# Create the client with your credentials
client = OAuth2::Client.new(UID, SECRET, site: "https://api.intra.42.fr")
# Get an access token
token = client.client_credentials.get_token
print "#{token}\n"
# info = token.get("/v2/cursus/42/users").parsed
# print "#{info}\n"
response = token.get("/v2/users/ikozlov")
print "#{response.parsed["location"]}\n"