#if !defined(COOKIES_H_)
#define COOKIES_H_

/****d* GalaxyNG/Cookies
 * NAME
 *   Cookies -- magic cookies used for sanity checks.
 * SOURCE
 */

#define  COOKIEplanet             0x01DFEE01
#define  COOKIEgroup              0x02DEEE12
#define  COOKIEbattle             0x03DDEE23
#define  COOKIEplayer             0x04DCEE34
#define  COOKIEalliance           0x05DBEE45
#define  COOKIEfleetname          0x06DAEE56
#define  COOKIEparticipant        0x07D9EE67
#define  COOKIEbatstat            0x08D8EE78
#define  COOKIEAplanet            0x09D7EE89
#define  COOKIEAalliance          0x0AD6EE9A
#define  COOKIEshiptype           0x0BD5EEAB
#define  COOKIEbombing            0x0CD4EEBC
#define  COOKIEplanet_claim       0x0DD3EECD
#define  COOKIEstrlist            0x0ED2EEDE
#define  COOKIEgame               0x0FD4EEDF
#define  COOKIEgamespecification  0xF1AABEEF
#define  COOKIEnewplayer          0xF1EA5747
#define  COOKIEgameOpts           0xF2AABEEF
#define  COOKIEplayerOpts	  0xF2EA5747
#define  COOKIEplanetTemplate     0xF2D4EEDF
#define  COOKIEemailList          0xF2FAFEED

/******/


#define   allocStruct(t) allocStructF(sizeof(t), COOKIE ## t)
#define   isStruct(t,p)  (p->cookie == COOKIE ## t)
#define   validateStruct(t,p) if(!(p->cookie == COOKIE ## t)) \
 { printf("validateStruct Failed in line %d of %s\n", __LINE__ , __FILE__);exit(100); } ;

void* allocStructF(unsigned int n, long cookie);

#endif

