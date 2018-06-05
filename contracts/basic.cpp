#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/asset.hpp>


using namespace eosio;
using namespace std;

class basic : public contract {
    using contract::contract;
    
public:
    basic( account_name self ) :
    contract(self),
    _product(_self,_self){}
    
    // @abi action
    void addproduct( account_name owner, string name, string description, asset price) {
        // cleos push action basic.app addproduct '["user1","wireless mouse","mac magic mouse","71.99 SYS"]' -p user1
        // cleos get table basic.app basic.app product
        require_auth(owner);

        eosio_assert(name != "","Name is required");
        eosio_assert(description != "","Description is required");
        eosio_assert(price.amount > 0,"Price must be greater than zero");
        
        _product.emplace(owner, [&] (auto& row) {
            row.id = _product.available_primary_key();
            row.owner = owner;
            row.name = name;
            row.description = description;
            row.price = price;
        });

    }

    // @abi action
    void modproduct(uint64_t id, string name, string description, asset price) {
        //cleos push action basic.app modproduct '[0,"Wireless Mouse","Mac Magic Mouse","72.99 SYS"]' -p user1

        eosio_assert(name != "","Name is required");
        eosio_assert(description != "","Description is required");
        eosio_assert(price.amount > 0,"Price must be greater than zero");

        auto iter = _product.find(id);
        require_auth(iter->owner);
        _product.modify( iter, 0, [&]( auto& row) {
            row.id = id;
            row.name = name;
            row.description = description;
            row.price = price;
        });
    }
    
    // @abi action
    void delproduct(uint64_t id) {
        // cleos push action basic.app delproduct '[1]' -p user1
        auto iter = _product.find(id);
        require_auth(iter->owner);
        _product.erase(iter);
    }

    // utility method for development
    // @abi action
    void clearproduct(uint64_t id) {
        // cleos push action basic.app clearproduct '[1]' -p basic.app
        require_auth(_self);
        while(_product.begin() != _product.end()) {
            auto itr = --_product.end();
            _product.erase(itr);
        }
    }

private:
    // @abi table
    struct product {
        uint64_t id;
        account_name owner;
        string name;
        string description;
        asset price;
        
        //uint64_t primary_key() const { return id; }
        auto primary_key()const { return id; }
        
        EOSLIB_SERIALIZE(product, (id)(owner)(name)(description)(price));
    };

    multi_index<N(product), product> _product;
    
    
    
};

EOSIO_ABI( basic, (addproduct)(modproduct)(delproduct)(clearproduct) );
