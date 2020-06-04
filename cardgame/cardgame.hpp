//the official tutorial's #include <eosiolib/eosio.hpp>
//will be deprecated in the next version of EOSIO.CDT.
//If you're using an old version of the CDT, you might
//need to modify the below line to eosiolib/eosio.hpp
#include <eosio/eosio.hpp>

using namespace std;
using namespace eosio;

CONTRACT cardgame : public eosio::contract {

  public:

    cardgame( name receiver, name code, datastream<const char*> ds ):
      contract(receiver, code, ds)
      {}

};