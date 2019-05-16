#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/print.hpp>
#include <eosio/crypto.hpp>

using namespace eosio;
using namespace std;

CONTRACT uuiduuiduuid : public eosio::contract {

  public:
    using contract::contract;

    ACTION add (std::string uuid) {
      entries_table entries(_self, _self.value);
      entries.emplace(_self, [&]( auto& row ) {
        row.id = entries.available_primary_key();
        row.uuid_str = uuid;
        row.uuid_int128 = get_int128_from_uuid_str(uuid);
        row.uuid_sha256 = get_sha256_from_str(uuid);
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
    TABLE entry {
      uint64_t id;
      std::string uuid_str;
      uint128_t uuid_int128;
      checksum256 uuid_sha256;
      uint64_t primary_key() const { return id; }
      uint128_t get_int128() const { return uuid_int128; }
      checksum256 get_sha256() const { return uuid_sha256; }

      EOSLIB_SERIALIZE( entry, (id)(uuid_str)(uuid_int128)(uuid_sha256));
    };

    typedef eosio::multi_index<"entries"_n, entry,
      eosio::indexed_by<"uuidint"_n, eosio::const_mem_fun<entry, uint128_t, &entry::get_int128>>,
      eosio::indexed_by<"uuidhash"_n, eosio::const_mem_fun<entry, checksum256, &entry::get_sha256>>
    > entries_table;

    uint128_t get_int128_from_uuid_str(std::string uuid){
      print("get_int128_from_uuid_str --> " + uuid + " --> ");
      std::string hexuuid;
      hexuuid.reserve(uuid.size());
      for(size_t i = 0; i < uuid.size(); ++i)
        if(uuid[i] != '-') hexuuid += uuid[i];
      print(hexuuid + " --> ");
      hexuuid = "Ox"+hexuuid;
      print(hexuuid  + "  --> ");
      uint128_t indexable_uuid = std::strtoull(hexuuid.c_str(), 0, 16);
      print(indexable_uuid);
      return indexable_uuid;
    }

    checksum256 get_sha256_from_str(const std::string datastr) {
      checksum256 result = sha256(datastr.data(), datastr.size() * sizeof(char));
      return result;
    }
};

EOSIO_DISPATCH( uuiduuiduuid,(add)(erase));
