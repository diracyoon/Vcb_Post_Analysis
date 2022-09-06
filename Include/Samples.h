#ifndef __Samples_H__
#define __Samples_H__

#include <map>

#include <TObject.h>
#include <TString.h>

using namespace std;

class Samples : public TObject
{
 public:
  Samples(const TString& a_era, const TString& a_channel);
  ~Samples();

  map<TString, TString> map_mc;
  map<TString, TString> map_data;

  map<TString, TString> map_short_name_mc;
  map<TString, TString> map_short_name_data;
  
 protected:
  TString era;
  TString channel;

  ClassDef(Samples, 1);
};

#endif /* __Samples_H__ */
