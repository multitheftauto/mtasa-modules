# ml_bcrypt - MTA bcrypt module

Bcrypt module for MTA:SA, for your passwords. Just three handy functions: `bcrypt_digest`, `bcrypt_salt`, and `bcrypt_verify`.

## Compiling
### Windows
```
premake5.exe vs2015
```
The project files are available in `Build/` then.

### Linux
```
./premake5 gmake

# Use either of the following commands
make all # Builds all (both debug and release for x86 and x64 - you'll need gcc-multilib then, not recommended - use one of the commands below instead)
make config=release_x86 all # Release build for the x86 platform
make config=release_x64 all # Release build for the x86_64 platform
```

## Documentation
### bcrypt_digest
    string bcrypt_digest(string key, string salt)
Returns the hash.

### bcrypt_salt
    string bcrypt_salt(int logRounds)
Please visit [this link](http://security.stackexchange.com/questions/17207/recommended-of-rounds-for-bcrypt) to determine the number of rounds appropriate for your server.
Returns the salt.

### bcrypt_verify
    bool bcrypt_verify(string key, string digest)
Returns whether it is verified. [How does it get the salt?](http://stackoverflow.com/a/6833165/1517394)

### Example
Here's some code that explains the use of all these functions, remember that the database functions mentioned in this aren't real functions and are just for this demonstration.
```lua
-- Get this information by conventional means
myName = "qaisjp"
myRegisterPassword = "LoLIcon"

-- When registering
-- A higher amount of rounds might result in your server freezing for several seconds/minutes
-- Dev notes: A rewrite of the resource should use a separate thread for the log rounds
mySalt = bcrypt_salt(15) 
hashedPassword = bcrypt_digest(myRegisterPassword, mySalt)
savePasswordInDatabase(myName, hashedPassword)

-- Now I want to login
myLoginPassword = "LoLIcon"
if bcrypt_verify(hashedPasswordFromDatabase, myLoginPassword) then
    outputChatBox("Password verified")
end
```
