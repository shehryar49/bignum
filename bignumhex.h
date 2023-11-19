#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
/*
TODO
----
Improve multiplication and division algos
Overload more operators
*/
class bignum//hexadecimal bignum
{
private:
  vector<uint8_t> bytes;//bytes stored in little endian format
  bool sign=false;
public:
  bignum(string s)
  {
     size_t len = s.length();
     size_t k = (int)len -1;
     uint8_t byte = 0;
     for(size_t j=1;j<=len;j++,--k)
     {
        uint8_t ch = s[k];
        if(ch>='0' && ch<='9')
        {
          ch-=48;
          if(j%2==0)
            ch<<=4;
        }
        else
        {
          ch-=87;
          if(j%2==0)
            ch<<=4;
        }
        byte = byte | ch;
        if(j%2==0 || j==s.length())
        {
          bytes.push_back(byte);
          byte = 0 ;
        }
     }
  }
  bignum(const bignum& rhs)
  {
    sign = rhs.sign;
    for(auto e: rhs.bytes)
      bytes.push_back(e);
  }
  
  bignum& operator=(const bignum& rhs)
  {
    bytes.clear();
    sign = rhs.sign;
    for(auto e: rhs.bytes)
    {
      bytes.push_back(e);
    }
    return *this;
  }
  bignum operator+(const bignum& rhs)const
  {
    bignum res("0");
    size_t k = 0;
    uint8_t byte1 = 0,byte2 = 0;
    bool carry = 0; //max carry that can go from one byte to another is 1
    uint8_t a=0;
    uint8_t b=0;
    bool nextcarry;
    res.bytes.pop_back();
    while(k<bytes.size() && k<rhs.bytes.size())
    {
        byte1 = bytes[k];
        byte2 = rhs.bytes[k];
        b = 255-byte2;
        if(byte1 > b || (byte1+byte2==255 && carry)) //carry generated
        {
            nextcarry = true;
            byte1 = byte1-b-1;
        }
        else
        {
          byte1+=byte2;
          nextcarry=false;
        }
        //max carry from previous byte is 1
        //and max value we get after adding byte1 and byte2 is 0xfe i.e 254
        //so adding carry won't overflow
        if(carry)
          byte1+=1;
        res.bytes.push_back(byte1);
        carry = nextcarry;
        k+=1;
    }
    while(k<bytes.size())
    {
        byte1 = bytes[k];
        if(carry)
        {
          if(byte1==255)
            byte1 = 0;
          else
          {
            byte1+=1;
            carry = false;
          }
        }
        res.bytes.push_back(byte1);
        k++;
    }
    while(k<rhs.bytes.size())
    {
        byte1 = rhs.bytes[k];
        if(carry)
        {
          if(byte1==255)
            byte1 = 0;
          else
          {
            byte1+=1;
            carry = false;
          }
        }
        res.bytes.push_back(byte1);
        k++;
    }
    if(carry)
    {
      res.bytes.push_back(1);
    }
    return res;
  }
  void increment()
  {
    size_t k = 0;
    uint8_t byte1 = 0;
    bool carry = 1; //max carry that can go from one byte to another is 1
    
    while(k<bytes.size())
    {
        byte1 = bytes[k];
        if(carry)
        {
          if(byte1==255)
            byte1 = 0;
          else
          {
            byte1+=1;
            carry = false;
          }
        }
        bytes[k] = byte1;
        k++;
    }
    if(carry)
    {
      bytes.push_back(1);
    }
    return;
  }
  bignum operator<<(int n)
  {
    uint8_t mask = 0x80;
    bool carry = false;
    bool nextcarry = false;
    bignum res = *this;
    for(int i=0;i<n;i++)
    {
      for(int j=0;j<res.bytes.size();++j)
      {
         i = res.bytes[j];
         nextcarry = (i & mask);
         i<<=1;
         if(carry)
           i = i|1;
         res.bytes[j] = i;
         carry = nextcarry;
      }
      if(carry)
        res.bytes.push_back(1);
    }
    return res;
  }
  bignum operator>>(int n)
  {
    uint8_t mask = 0x80;
    bool carry = false;
    bool nextcarry = false;
    bignum res = *this;
    for(int i=0;i<n;i++)
    {
      for(int j=res.bytes.size()-1;j>=0;--j)
      {
         i = res.bytes[j];
         nextcarry = (i & (uint8_t)1);
         i>>=1;
         if(carry)
           i = i|mask;
         res.bytes[j] = i;
         carry = nextcarry;
      }
      if(carry)
        res.bytes.push_back(0x80);
    }
    return res;
  }
  void printBin()
  {
    for(auto e: bytes)
    {
        uint8_t m = 1;
        cout<<"byte: "<<(int)e<<endl;
        
    }
  }
  bignum operator*(const bignum& rhs)const
  {
    bignum addend = *this;
    bignum res("0");
    int n = 0;
    for(auto e: rhs.bytes)
    {
        uint8_t m = 1;
        for(int i=1;i<=8;i++)
        {
            bool bit = e&m;
            if(e & m)
            {
              res = res+addend;
            }
            addend = addend<<1;
            m<<=1;
            n+=1;
        }
    }
    return res;
  }
  bool operator<(const bignum& rhs)const
  {
    if(bytes.size() < rhs.bytes.size())
      return true;
    else if(bytes.size() > rhs.bytes.size())
      return false;
    for(int i =bytes.size()-1;i>=0;--i)
    {
        if(bytes[i] < rhs.bytes[i])
          return true;
        else if(bytes[i] > rhs.bytes[i])
          return false;
    }
    return false;
  }
  bool operator>(const bignum& rhs)const
  {
    if(bytes.size() > rhs.bytes.size())
      return true;
    else if(bytes.size() < rhs.bytes.size())
      return false;
    for(int i =bytes.size()-1;i>=0;--i)
    {
        if(bytes[i] > rhs.bytes[i])
          return true;
        else if(bytes[i] < rhs.bytes[i])
          return false;
    }
    return false;
  }
  inline bool operator<=(const bignum& rhs)const
  {
    return !(*this > rhs);
  }
  inline bool operator>=(const bignum& rhs)const
  {
    return !(*this < rhs);
  }
  
  bool operator==(const bignum& rhs)const
  {
    return bytes == rhs.bytes;
  }
  bool operator!=(const bignum& rhs)const
  {
    return bytes != rhs.bytes;
  }
  
  bignum operator-(const bignum& rhs)const
  {
    if(*this < rhs)
    {
      bignum res = rhs-*this;
      res.sign = true;
      return res;
    }
    //do *this - rhs
    size_t k=0;
    bool carry = false;
    bool nextcarry = false;
    bignum res("0");
    res.bytes.clear();
    uint8_t c;
    for(;k<rhs.bytes.size();k++)
    {
      uint8_t a = bytes[k];
      uint8_t b = rhs.bytes[k];
      if(carry)
      {
        if(a==0)
        {
          a = 255;
          nextcarry = true;
        }
        else
        {
          a-=1;
          nextcarry = false;
        }
      }
      if(a < b)
      {
        c = 256 - (b-a);
        nextcarry = true;
      }
      else
        c = a-b;
      res.bytes.push_back(c);
      carry = nextcarry;
    }
    size_t count = 0;
    for(;k<bytes.size();++k)
    {
      uint8_t a = bytes[k];
      if(carry)
      {
        if(a==0)
        {
          a=255;
        }
        else
        {
          a-=1;
          carry = false;
        }
      }
      if(a==0)
       ++count;
      else
        count = 0;
      res.bytes.push_back(a);
    }
    res.bytes.erase(res.bytes.end()-count,res.bytes.end());
    return res;
  }
  bignum operator/(const bignum& rhs)const
  {
    bignum k = *this;
    bignum quotient("0");
    bignum one("1");
    while(k>=rhs)
    {
      quotient = quotient+one;
      k = k-rhs;
    }
    return quotient;
  }
  bignum operator%(const bignum& rhs)const
  {
    bignum k = *this;
    bignum one("1");
    while(k>=rhs)
    {
      k = k-rhs;
    }
    return k;
  }
  
  string val() //returns hex string of bignumber
  {
    string s = "";
    uint8_t mask1 = 0xf0;
    uint8_t mask2 = 0x0f;
    uint8_t i,j;
    for(auto e: bytes)
    {
       i = e & mask1;
       j = e & mask2;
       i = i>>4;
       if(j<=9)
         s+=(char)(48+j);
       else 
         s+=(char)(87+j);
       if(i<=9)
         s+=(char)(48+i);
       else 
         s+=(char)(87+i);
    }
    std::reverse(s.begin(),s.end());
    return ((sign) ? "-0x" : "0x") +s;
  }
};