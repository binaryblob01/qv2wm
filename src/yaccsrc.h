typedef union {
  char* str;
  MenuElem* mItem;
  AttrStream* aStream;
  unsigned int modifier;
} YYSTYPE;
#define	VARIABLE	258
#define	MENU	259
#define	SHORTCUT	260
#define	APP	261
#define	KEY	262
#define	IND	263
#define	EXITDLG	264
#define	STARTUP	265
#define	ACC	266
#define	VAR	267
#define	STRING	268
#define	FUNC	269
#define	PLUS	270
#define	MINUS	271


extern YYSTYPE yylval;
