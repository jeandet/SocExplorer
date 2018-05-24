#include "memtester.h"
#include <socexplorerengine.h>
#include <socexplorerproxy.h>

MemTester::MemTester(QObject* parent)
  :QObject(parent)
{

}

MemTester::~MemTester()
{

}


unsigned int p_pow2(unsigned int v)
{
  static const char LogTable256[256] =
  {
  #define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
      -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
      LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
      LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
  };

  unsigned int r;     // r will be lg(v)
  register unsigned int t, tt; // temporaries

  if (tt = v >> 16)
  {
    r = (t = tt >> 8) ? 24 + LogTable256[t] : 16 + LogTable256[tt];
  }
  else
  {
    r = (t = v >> 8) ? 8 + LogTable256[t] : LogTable256[v];
  }
  return r;
}

unsigned int MemTester::measureMemSize(socexplorerplugin *plugin, unsigned int address, unsigned int maxSize)
{
  unsigned int curVal=1,testAddress=address;
  unsigned int size=0;
  if(Q_UNLIKELY(!plugin || !plugin->isConnected()))return 0;
  plugin->Write(&curVal,1,testAddress);
  plugin->Read(&curVal,1,testAddress);
  if(Q_UNLIKELY(curVal!=1))
    return size;
  unsigned int max= p_pow2((0xFFFFFFFFFFFFFFFF - address))+1;
  register unsigned int maxSizeLg=p_pow2(maxSize)-1;
  if(max>maxSizeLg)
    max=maxSizeLg;
  if(max>32)max=32;
  for(size=2;size<max;size++) //check each address bit
    {
      testAddress = (unsigned int)(address+(1<<size));
      curVal = (unsigned int)(1<<size);
      plugin->Write(&curVal,1,testAddress);
      if(!MemTester::m_check_space(plugin,address,size))
          return (1<<(size));
    }
  return (1<<(size+1));
}

unsigned int MemTester::measureMemSize(const QString &plugin, unsigned int address, unsigned int maxSize)
{
    return measureMemSize(socexplorerproxy::findPlugin(plugin),address,maxSize);
}

bool MemTester::m_check_space(socexplorerplugin *plugin,unsigned int address, unsigned int pow)
{
    unsigned int currentAddress=4;
    unsigned int curVal=0xffffff;
    while (currentAddress<=(1<<pow))
    {
        plugin->Read(&curVal,1,address+currentAddress);
        if(curVal!=currentAddress)
            return false;
        currentAddress = currentAddress <<1;
    }
    return  true;
}


