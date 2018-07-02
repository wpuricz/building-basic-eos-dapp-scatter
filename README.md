#### Basic EOS dapp example using scatter 

#### Config

Create a file called eosconfig.js and add it to the config directory. Paste this into the file and add your actual endpoint and keys for the app:

```
module.exports = {
    scope:"{accountnamehere}",
    code:"{accountnamehere}",
    protocol: "http",
    url:"x.x.x.x",
    port: 8888,
    chainId:"{chainidhere}",
  };
```

Start using: 
    npm install
    npm run dev


##### Note: This is using EOSJS 14.1
