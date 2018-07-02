<template>
  <div class="Hello">
    {{scatterMessage}}
    
     <div class="product-form">
      <!-- <label for="rowid">ID</label>
      <input name="rowid" v-model="rowid" /><br/> -->
      
      <label for="name">Name</label>
      <input name="name" v-model="name"/><br/>
      <label for="description">Description</label>
      <input name="description" v-model="description"/><br/>
      <label for="price">Price</label>
      <input name="price" v-model="price"/><br/>
    </div>
      
    <div v-if="editMode">
      <b-button @click="editProduct" size="" variant="primary">Edit Product</b-button>  
      <b-button @click="cancelEdit" size="" variant="danger">Cancel</b-button>  
    </div>
    <div v-else>
      <b-button @click="addProduct" size="" variant="primary">Add Product</b-button>
    </div>

    <div v-if="processing" class="alert alert-light processing" role="alert">
      Processing...
    </div>


    <table  class="table table-bordred table-striped product-table">
      <tr>
        <th>ID</th><th>Seller</th><th>Product</th> <th>Description</th><th>Price</th> <th>Edit</th><th>Delete</th>
      </tr>
      <tr v-for="(row,index) in rows" :key="row.id">
        <td>{{ row.id }}</td>
        <td>{{ row.owner }}</td>
        <td>{{ row.name }}</td>
        <td>{{ row.description }}</td>
        <td>{{ row.price }}</td>
        <td><b-button @click="edit(index)" size="sm" variant="primary">Edit</b-button></td>
        <td><b-button @click="deleteProduct(row.id)" size="sm" variant="danger">X</b-button></td>
      </tr>
    </table>

  </div>
</template>

<script>

import * as Eos from 'eosjs';
import eosconfig from '../../../config/eosconfig'

let scatterInstalled = false;

const network = {
  blockchain:'eos',
  protocol: eosconfig.protocol,
  host: eosconfig.url, 
  port: eosconfig.port, 
  chainId: eosconfig.chainId 
}

export default {
  name: 'hello',
  data() {
    return {
      msg: 'Building a Basic EOS Dapp',
      rows:[],
      rowid:0,
      name:'',
      description:'',
      price:'',
      processing:false,
      editMode:false,
      contract:null,
      scatterMessage: "",
      eos:null,
      currentUser:"",
      options:null
    };
  },
  created: async function() {
    
    setTimeout(() => {
      if(!scatterInstalled) {
        this.scatterMessage = "Scatter is not installed, please install and refresh page";
      }
    },2000);

    document.addEventListener('scatterLoaded', scatterExtension => {
      scatterInstalled = true;
      const scatter = window.scatter;

      scatter.getIdentity({accounts:[network]}).then(identity => {
        
        const account = scatter.identity.accounts.find(account => account.blockchain === 'eos')
        const authorization = {actor:account.name, permission:account.authority};
        this.options = { authorization: [authorization] }
        
        this.eos = scatter.eos(network,Eos,{ chainId: network.chainId});
        
        this.currentUser = scatter.identity.accounts[0].name;
        this.scatterMessage = "Scatter detected, Welcome " + this.currentUser;

        this.eos.contract(eosconfig.code)
        .then(contract => {
          this.contract = contract;
          this.getProducts();

        })

      })
      .catch(error => {
        console.log(JSON.stringify(error));
        if(error.type === "locked") {
          this.scatterMessage = "Please unlock scatter and refresh the page";
        }else{
          this.scatterMessage = "No scatter identity found, please add identity and refresh the page";
        }
        
      })
      
    })

  },
  methods: {
    
    getProducts: async function() {
      let tableQuery = {
        "json":true,
        "scope":eosconfig.scope,
        "code":eosconfig.code,
        "table":"product"
      }
      
      let result = await this.eos.getTableRows(tableQuery);
      this.rows = result.rows;

    },
    addProduct: async function() {
      
      try {
        this.processing = true;
        
        let response = await this.contract.addproduct(
            this.currentUser, 
            this.name, 
            this.description, 
            this.price + ' SYS', 
            this.options
          );
        
        let result = await this.getProducts();
        this.processing = false;

      }catch(e) {
        alert(JSON.parse(e).error.details[0].message)
        
      }

    },
    deleteProduct: async function(rowid) {
      this.processing = true
      let result = await this.contract.delproduct(
        rowid,
        this.options
      )
      
      let response = await this.getProducts();
      this.processing = false
    },
    editProduct: async function() {
      
      this.processing = true
      let result = await this.contract.modproduct(
        this.rowid,
        this.name,
        this.description,
        this.price,
        this.options
      )
      
      let response = await this.getProducts()
      this.processing = false
      this.resetForm()
      this.editMode = false
    },
    edit: function(index) {
      let currentRow = this.rows[index]
      this.rowid = currentRow.id
      this.name = currentRow.name;
      this.description = currentRow.description
      this.price = currentRow.price
      this.editMode = true;
    },
    resetForm: function() {
      this.rowid = 0
      this.name = "";
      this.description = ""
      this.price = ""
    },
    cancelEdit: function() {
      this.resetForm()
      this.editMode = false
    }
  }

  
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
h1, h2 {
  font-weight: normal;
}

ul {
  list-style-type: none;
  padding: 0;
}

li {
  display: inline-block;
  margin: 0 10px;
}

a {
  color: #42b983;
}

.product-form {
  margin-top:10px;
}
.product-table {
  margin-top: 50px;
}

.processing {
  margin:auto;
  width:150px
}



</style>
