#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;
class bignum
{
private:
  string val;
  bool sign;
public:
  bignum()
  {
    val = "0";
    sign = false;
  }
  bignum(const std::string& str)
  {
    if(str.length() == 0)
      throw std::logic_error("empty string not allowed!");
    auto it = str.rbegin();
    auto rend = str.rend();
    sign =false;
    while(it != rend)
    {
        char ch = *it;
        if(ch < '0' || ch > '9') //not a digit
        {
          if(ch == '-' && ++it == rend)
          {
            sign = true;
            break;
          }
          throw std::logic_error("only digits allowed in bignum value!");
        }
        val.push_back(ch);
        it++;
    }
    
  }
  bignum(const bignum& obj)
  {
    val = obj.val;
    sign = obj.sign;
  }
  bignum operator+(const bignum& rhs)
  {
    bignum res;
    res.val = "";
    size_t i = 0;
    size_t j = 0;
    size_t l1 = this->val.length();
    size_t l2 = rhs.val.length();
    uint8_t carry = 0;
    while(i<l1 && j<l2)
    {
        uint8_t k = (val[i]-48) + (rhs.val[j]-48) + carry;
        if(k>=10)
        {
          k-=10;
          carry = 1;
        }
        else
          carry = 0;
        res.val.push_back((char)(k+48));
        i++;
        j++;
    }
    while(i<l1)
    {
       uint8_t k = val[i]-48 + carry;
       if(k>=10)
       {
        k-=10;
        carry = 1;
       }
       else
         carry = 0;
       res.val.push_back((char)(k+48)); 
       i++;
    }
    while(j<l2)
    {
       uint8_t k = rhs.val[j]-48 + carry;
       if(k>=10)
       {
        k-=10;
        carry = 1;
       }
       else
         carry = 0;
       res.val.push_back((char)(k+48)); 
       j++;
    }
    if(carry)
      res.val.push_back('1');
    return res;
  }
  bool operator==(const bignum& rhs)
  {
    return (sign == rhs.sign) && (val == rhs.val) ;
  }
  bool operator!=(const bignum& rhs)
  {
    return !((sign == rhs.sign) && (val == rhs.val)) ;
  }
  bool operator<(const bignum& rhs)
  {
    size_t l1 = val.length();
    size_t l2 = rhs.val.length();
    if(sign && !rhs.sign)
      return true;
    if(!sign && rhs.sign)
      return false;
    //signs are same

    if(l1 < l2 )
      return !sign;
    else if(l1 > l2)
      return sign;
    //lengths are equal
    size_t k = l1 - 1;
    size_t i = 1;
    bool equal = false;
    while(i<=l1)
    {
      if(val[k] > rhs.val[k])
        return sign;
      equal = val[k] == rhs.val[k];
      i++;
      k--;
    }
    return !equal;
  }
  bool operator<=(const bignum& rhs)
  {
    size_t l1 = val.length();
    size_t l2 = rhs.val.length();
    if(sign && !rhs.sign)
      return true;
    if(!sign && rhs.sign)
      return false;
    //signs are same

    if(l1 < l2 )
      return !sign;
    else if(l1 > l2)
      return sign;
    //lengths are equal
    size_t k = l1 - 1;
    size_t i = 1;
    while(i<=l1)
    {
      if(val[k] > rhs.val[k])
        return sign; 
      i++;
      k--;
    }
    return true;
  }
  bool operator>(const bignum& rhs)
  {
     return !(*this <= rhs);
  }
  bool operator>=(const bignum& rhs)
  {
    return !(*this < rhs);
  }
  bignum operator-(const bignum& rhs)
  {
    if(sign && !rhs.sign)
    {
      bignum res =  *this + rhs;
      res.sign = true;
      return res;
    }
    static string a,b;
    bignum res;
    res.val = "";
    if(*this < rhs)
    {
      a = rhs.val;
      b = val;
      res.sign = true;
    }
    else
    {
      a = val;
      b = rhs.val;
    }
    //compute a-b
    
    int8_t borrow = 0;
    size_t l1 = a.length();
    size_t l2 = b.length();
    size_t i = 0;
    size_t j = 0;
    while(i<l1 && j<l2)
    {
      int8_t ans = (a[i]-48) - (b[j]-48) + borrow;
      if(ans < 0)
      {
        borrow = -1;
        ans += 10;
      }
      else
        borrow = 0;
      
      res.val.push_back((char)(48+ans));
      i++;
      j++;
    }
  
    //if anything has greater length, it's a
    while(i < l1)
    {
      int8_t ans = (a[i] - 48) + borrow;
      if(ans < 0)
      {
        borrow = -1;
        ans += 10;
      }
      else
        borrow = 0;
      res.val.push_back((char)(48+ans));
      i++;
    }
    while(res.val.length() > 1 && res.val.back() == '0')
      res.val.pop_back();
    return res;
  }
  void increment()
  {
    uint8_t carry = 1;
    size_t i = 0;
    size_t l = val.length();
    while(i<l)
    {
      uint8_t k = (val[i] - 48) + carry;
      if(k >= 10)
      {
        k-=10;
        carry = 1;
        val[i] = (char)(k+48);
      }
      else
      {
        val[i] = (char)(k+48);
        carry = 0;
        break;
      }
      i++;
    }
    if(carry)
      val.push_back('1');

  }
  const string& str()
  {
    static string tmp;
    tmp =  val;
    std::reverse(tmp.begin(),tmp.end());
    if(sign)
      tmp = "-"+tmp;
    return tmp;
  }

};
int main()
{
/*  bignum a("123343421");
  bignum b("4573489");
  bignum c = a+b;
  printf("%s\n",c.str().c_str());*/
  /*
  bignum k("0");
  bignum x("1000");
  for(int i=1;i<=100000;i++)
    k.increment();
  printf("%s\n",k.str().c_str());*/

/*  bignum a("1");
  bignum b("100000");
  printf("%s\n",a.str().c_str());
  while(a<=b)
    a.increment();

  printf("%s\n",a.str().c_str());
  printf("%s\n",b.str().c_str());
  
  printf("a<=b: %d\n",(a<=b));*/
  bignum a("0");
  bignum b("-1000");
  bignum one("1");
  bignum c = a-b;
//  printf("a>=b = %d\n",(a>=b));
//  return 0;
  while(a >= b)
  {
  printf("a = %s\n",a.str().c_str());
      a = a-one;
  }
  printf("a = %s\n",a.str().c_str());
}
/*

*/