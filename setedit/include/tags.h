/* Copyright (C) 2003 by Salvador E. Tropea (SET),
   see copyrigh file for details */

#ifndef Uses_TagsOnlyFuncs
typedef unsigned char uchar;
class TSpTagCollection;
class TStringCollection;
class TTagClassCol;

// Flags for a tag
const uchar
      sttFgLine=1,  // != regex
      sttFgLocal=2, // != global

      sttFgClass   =0x04, // 3 bits
      sttFgEnum    =0x08,
      sttFgUnion   =0x0C,
      sttFgInherits=0x10,
      sttFgStruct  =0x14,
      sttFgPMask   =0x1C,

      sttFgVirtual    =0x20,
      sttFgPureVirtual=0x40,
      sttFgAbstract   =0x60;

struct stTagFile;
class  TTagInfo;

// A tag
struct stTag
{
 const char *id;
 const char *source;
 union
 {
  const char *regex;
  unsigned line;
 };
 const char *partof;
 stTagFile  *tagFile;
 uchar lang;
 uchar kind;
 uchar flags;
};

// To hold one of the description variables found at the beggining of
// the tag files.
struct stTagInfo
{
 const char *var;
 const char *value;
 const char *comment;
};

// Information about one tag files
struct stTagFile
{
 const char *file;
 const char *base;
 time_t      modtime;
 int         entries;
 TTagInfo   *info;
};

// Structures to convert the single letter "kind" field to a descriptive text
struct stTagKind
{
 uchar kind;
 const char *name;
};

struct stTagKinds
{
 unsigned count;
 stTagKind *kinds;
};

struct stClassTagInfo
{
 stTag *cl;
 TStringCollection *parents;
 TStringCollection *childs;
 TSpTagCollection  *members;
};

// Class to handle the variables for the tag file
class TTagInfo : public TStringCollection, public TStringable
{
public:
 TTagInfo() : TStringCollection(6,2) {};
 int addValue(char *s);
 virtual void freeItem(void *item);
 virtual void *keyOf(void *item);
 void print() { forEach(print1,NULL); };
 static void print1(void *item, void *arg);
 stTagInfo *atPos(ccIndex pos) { return (stTagInfo *)at(pos); };
 // TStringable things:
 virtual void getText(char *dest, unsigned item, int maxLen);
 virtual unsigned GetCount(void) { return getCount(); };
};

// Class for the tag files
class TTagFiles : public TStringCollection, public TStringable
{
public:
 TTagFiles();
 virtual void freeItem(void *item);
 virtual void *keyOf(void *item);
 void print() { forEach(print1,NULL); };
 static void print1(void *item, void *arg);
 stTagFile *atPos(ccIndex pos) { return (stTagFile *)at(pos); };
 // TStringable things:
 virtual void getText(char *dest, unsigned item, int maxLen);
 virtual unsigned GetCount(void) { return getCount(); };

 //SetDefStreamMembers(TTagFiles,TStringCollection);
};

//SetDefStreamOperators(TTagFiles);

// Class to hold the tags
class TSpTagCollection : public TStringCollection, public TStringable
{
public:
 TSpTagCollection(unsigned size);
 ~TSpTagCollection();
 int addValue(char *s, stTagFile *tf);
 virtual void  freeItem(void *item);
 virtual void *keyOf(void *item);
 virtual int   compare(void *key1, void *key2);

 // TStringable things:
 virtual void getText(char *dest, unsigned item, int maxLen);
 virtual unsigned GetCount(void) { return getCount(); };

 stTag *atPos(ccIndex pos) { return (stTag *)at(pos); };
 static const char *getKind(stTag *p);
 static const char *getLanguage(stTag *p);
 static void        getText(char *buf, void *item, int maxLen);

protected:
 TStringCollection *files;
 static const char *Languages[];
 static stTagKinds  Kinds[];
};

class TTagMembersCol : public TSpTagCollection
{
public:
 TTagMembersCol();
 ~TTagMembersCol();
 virtual void getText(char *dest, unsigned item, int maxLen);
 void insert(stTag *tg, int level);
 void insertSorted(stTag *tg, int level);
 void collect(stClassTagInfo *p, TTagClassCol *clist,
              int level=0, Boolean sort=False);

protected:
 TNSCollection *levels;
 void collectFromOne(TSpTagCollection *c, int level, Boolean sort);
};

class TTagCollection : public TSpTagCollection
{
public:
 TTagCollection();
 ~TTagCollection();
 int addFile(const char *file, int defer=0);

 int refresh();
 void print() { forEach(print1,NULL); };
 static void print1(void *item, void *arg);
 TStringCollection *getTagFilesList();
 void               deleteTagsFor(stTagFile *p);
 int                save(fpstream& s);
 int                load(fpstream& s);

 TTagFiles         *tagFiles;

protected:
 int loadTagsFromFile(stTagFile *p);
 int abortInit;
};

class TTagClassCol : public TStringCollection
{
public:
 TTagClassCol(TSpTagCollection *from);

 stClassTagInfo *atPos(ccIndex pos) { return (stClassTagInfo *)at(pos); };
 virtual void  freeItem(void *item);
 virtual void *keyOf(void *item);
 Boolean searchId(const char *id, ccIndex &pos) { return search((void *)id,pos); };

 void addClass(stTag *p);
 void addMember(stTag *p);
 void addChildTo(const char *parent, const char *child);
 stTag *newFake(const char *id);
 void deleteFake(stTag *p);
 stClassTagInfo *newClass();
 stClassTagInfo *getClassOrFake(const char *id);
};
#endif

int  TagsSave(fpstream& s);
int  TagsLoad(fpstream& s);
void TagsFreeMemory();
void EditTagFiles();
void SearchTag(char *word);
void TagsClassBrowser(char *word);
char *TagsWordCompletion(int x, int y, char *word);

