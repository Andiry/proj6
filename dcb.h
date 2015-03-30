// 
//
//

struct DCB_QWord
{
  union {
	unsigned int dword[2];
    unsigned int word[4];
    unsigned char byte[8];
  };
};
typedef struct DCB_QWord dcb_qword_t;
  
struct DCB
{
  union {
     dcb_qword_t qword[4];
     unsigned char byte[32];
  };
};	
typedef struct DCB dcb_t;
