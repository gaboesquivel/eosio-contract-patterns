#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/print.hpp>
#include <eosio/crypto.hpp>

using namespace eosio;
using namespace std;

CONTRACT structstruct : public eosio::contract {

  public:
    using contract::contract;

    ACTION add (name account) {
      vector<name> accounts{ account };
      struct_b b = { accounts };
      vector<struct_b> vector_b{ b };
      struct_a a = { vector_b };

      entries_table entries(_self, _self.value);
      entries.emplace(_self, [&]( auto& row ) {
        row.id = entries.available_primary_key();
        row.vector_a.push_back(a);
      });
    }

    ACTION erase() {
      // only contract owner can erase table
      require_auth(_self);
      entries_table entries(_self, _self.value);
      auto entries_itr = entries.begin();
      while ( entries_itr != entries.end()) {
        entries_itr = entries.erase(entries_itr);
      }
    }

  private:
    struct struct_b {
      vector<name> accounts;
    };

    struct struct_a {
      vector<struct_b> vector_b;
    };

    TABLE entry {
      uint64_t id;
      vector<struct_a> vector_a;

      uint64_t primary_key() const { return id; }

      EOSLIB_SERIALIZE( entry, (id)(vector_a));
    };

    typedef eosio::multi_index<"entries"_n, entry > entries_table;
};

EOSIO_DISPATCH( structstruct,(add)(erase));
