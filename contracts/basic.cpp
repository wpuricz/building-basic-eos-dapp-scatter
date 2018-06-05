#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/asset.hpp>


using namespace eosio;
using namespace std;

class commerce : public contract {
    using contract::contract;
    
public:
    commerce( account_name self ) :
    contract(self),
    _product(_self,_self){}
    
    // @abi action
    void addproduct( name owner, string name, string description, asset price) {
        // cleos push action commerce.code addproduct '[2,"user1","wireless mouse","mac magic mouse","71.99 EOS"]' -p user1
        // cleos get table commerce.code commerce.code product
        require_auth(owner);
        
        _product.emplace(owner, [&] (auto& row) {
            row.id = _product.available_primary_key();
            row.owner = owner;
            row.name = name;
            row.description = description;
            row.price = price;
        });

    }

    // @abi action
    void modproduct(uint64_t id, name owner, string name, string description, asset price) {
        //cleos push action commerce.code modproduct '[2,"user1","wireless mouse","mac magic Mouse","71.99 EOS"]' -p user1
        require_auth(owner);
        auto iter = _product.find(id);
        _product.modify( iter, 0, [&]( auto& row) {
            row.id = id;
            row.owner = owner;
            row.name = name;
            row.description = description;
            row.price = price;
        });
    }
    
    // @abi action
    void delproduct(uint64_t id, name owner) {
        // cleos push action commerce.code dropproduct '[1]' -p user1
        require_auth(owner);
        auto iter = _product.find(id);
        _product.erase(iter);
    }
    
    
private:
    // @abi table
    struct product {
        uint64_t id;
        name owner;
        string name;
        string description;
        asset price;
        
        //uint64_t primary_key() const { return id; }
        auto primary_key()const { return id; }
        
        EOSLIB_SERIALIZE(product, (id)(owner)(name)(description)(price));
    };

    multi_index<N(product), product> _product;
    
    
    
};

EOSIO_ABI( commerce, (addproduct)(modproduct)(delproduct) );
