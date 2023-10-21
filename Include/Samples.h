#ifndef __Samples_H__
#define __Samples_H__

#include <map>

#include <TObject.h>
#include <TString.h>

using namespace std;

class Samples : public TObject
{
public:
  Samples(const TString &a_era, const TString &a_channel, const TString &a_analyser = "Vcb");
  ~Samples();

  map<TString, TString> map_mc;
  map<TString, TString> map_top_syst;

  vector<TString> vec_period;
  map<TString, TString> map_data;

  map<TString, TString> map_short_name_mc;
  map<TString, TString> map_short_name_data;

  map<TString, TString> map_short_short_name;

protected:
  TString era;
  TString channel;
  TString analyser;

  ClassDef(Samples, 1);
};

#endif /* __Samples_H__ */
