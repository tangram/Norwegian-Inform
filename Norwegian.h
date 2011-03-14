! ===========================================================================
!
!   Inform Language Definition File: Norwegian 20110903
!   Derived from Swedish.h dated 20080404
!   Compatible with English.h dated 040227
!
!   Supplied for use with Inform 6 -- Release 6/11 -- Serial number 040227
!
!   Copyright Graham Nelson 1993-2004 but freely usable (see manuals)
!   Based on the Swedish translation (c) Fredrik Ramsberg 2003-2008.
!   Norwegian translation by Eirik Krogstad 2011 (with permission).
!   All the hard work was done by Mr. Ramsberg, and he deserves due credit.
!   It is only fortunate that Scandinavian languages are so similar.
!
!   This translation has a license which should always be distributed
!   along with it. It basically says that you're free to use the translation,
!   as long as you don't sell it and don't claim that you've written it.
!   You may sell games compiled using this translation though.
!
!   This file is automatically Included in your game file by "parserm".
!   Strictly, "parserm" includes the file named in the "language__" variable,
!   whose contents can be defined by +language_name=XXX compiler setting (with
!   a default of "english").
!
! ---------------------------------------------------------------------------

! Comment conventions
! *#* means a problem not yet adressed
! �#� means a problem that's been solved

System_file;

#ifndef LIBRARY_VERSION;
  Message warning "This version of Norwegian.h was made for use with library 6/11.";
#endif;
#iftrue LIBRARY_VERSION < 611;
  Message warning "This version of Norwegian.h was made for use with library 6/11.";
#endif;

! ---------------------------------------------------------------------------
!   First we need to replace some routines
! ---------------------------------------------------------------------------

! Routines in Parserm.h, to be replaced by routines in NorRout.h
Replace ResetDescriptors;
Replace Descriptors;
Replace ScoreMatchL;
Replace PrintCommand;
Replace CantSee;
Replace GetGNAOfObject;
Replace PronounNotice;
Replace PrefaceByArticle;

Replace Indefart;
Replace CInDefart;
Replace Defart;
Replace CDefart;

! Routines in Verblibm.h, to be replaced by routines in NorRout.h

Replace WriteAfterEntry;
Replace Objects1Sub;

! ---------------------------------------------------------------------------
!   Part I.   Preliminaries
! ---------------------------------------------------------------------------

Constant LanguageVersion = "Norsk Inform v1.0 oversatt av Eirik Krogstad 2011, basert p� svensk oversettelse (c) Fredrik Ramsberg 2003-2008";

Global caps_mode;
Global transcribe_mode;

Attribute def_article;
Attribute uter;

Property short_name_def;

[check_caps obj   c k;
  c = k;
  if(metaclass(obj)==String)
    print (string) obj;
  else
    print (object) obj;
  rtrue;
];

[PrintBuffer buffer_name   i;
  for (i=0 : i<buffer_name-->0 : i++)
    print (char) buffer_name->(i+WORDSIZE);
];


#ifdef DEBUG;

Constant _MAXPARSE 15;
Array _NameTestBuffer -> (160 + 2);

#ifdef TARGET_ZCODE;

Array _BadWords -> _MAXPARSE;

#ifnot; !Glulx

Array _BadWords -> MAX_BUFFER_WORDS;

#endif;


[_CheckName o prop   i badcount;
  caps_mode=false;
  indef_mode=false;
  PrintToBuffer(_NameTestBuffer, 160, o, prop);
#ifdef TARGET_ZCODE;
  parse->0 = _MAXPARSE;
  for(i=2:i < (2 + _NameTestBuffer -> 1): i++) {
    _NameTestBuffer -> i = LowerCase(_NameTestBuffer -> i);
    if((_NameTestBuffer -> i)==','  or '.'  or '"')
    _NameTestBuffer -> i = ' ';
  }
#ifnot; !Glulx
  for(i=4:i < (4 + _NameTestBuffer --> 0): i++) {
    _NameTestBuffer -> i = LowerCase(_NameTestBuffer -> i);
    if((_NameTestBuffer -> i)==','  or '.'  or '"')
    _NameTestBuffer -> i = ' ';
  }
#endif;
  Tokenise__(_NameTestBuffer, parse);
  if(o.parse_name~=0) { ! Method 1: Check with object parse_name-routine
    parser_action = NULL;
    wn=1;
    i = RunRoutines(o, parse_name);
#ifdef TARGET_ZCODE;
    if(i < parse->1)
      _BadWords->(badcount++) = i;
  } else {! Method 2: Check the object name-property
    for(i=0:i < parse->1 && i < _MAXPARSE: i++)
      if(~~WordInProperty(parse-->(1+i*2), o, name))
        _BadWords->(badcount++) = i;
  }
#ifnot; !Glulx
    if(i < parse-->0)
      _BadWords->(badcount++) = i;
  } else {! Method 2: Check the object name-property
    for(i=0:i < parse-->0 && i < MAX_BUFFER_WORDS: i++)
      if(~~WordInProperty(parse-->(1+i*3), o, name))
        _BadWords->(badcount++) = i;
  }
#endif;
  return badcount;
];

[_PrintBadWords count  i j k;
  print "(";
  for(k=0: k<count: k++) {
    i = _BadWords -> k;
#ifdef TARGET_ZCODE;
    for(j=0: j < parse -> (4+i*4): j++) {
      print (char) _NameTestBuffer -> (j + parse->(5+i*4));
    }
#ifnot; !Glulx
    for(j=0: j < parse --> (2+i*3): j++) {
      print (char) _NameTestBuffer -> (j + parse-->(3+i*3));
    }
#endif;
    if(k < count - 1)
      print ", ";
  }
  print ")";
];

[_CheckThisObj obj   j isNormalObj;
  isNormalObj=parent(obj)~=0 || obj hasnt light;
  if(isNormalObj && parent(obj)==0) { ! Check if dark, lonely objects have exits
    objectloop(j in compass) isNormalObj=isNormalObj && (obj.(j.door_dir)==0);
  }
  return isNormalObj;
];

[CheckGameSub  i err anyerr wordno specific;
  specific=noun;
#ifdef TARGET_GLULX;
  if(specific==10000 && TryNumber(2)==10000) {
    ! Parse object number manually
    specific=0;
    for(i=0:i<WordLength(2):i++) {
      specific=10 * specific + ((WordAddress(2)->i) - '0');
    }
  }
#endif;
  style bold;
  if(specific) print "Unders�ker bare: ", (name) specific, "^";
  print  "Ser etter f�lgende indikatorer p� problem:
    ^* Mangler b�de short_name_def og short_name.
    ^* Har short_name OG short_name_indef eller short_name_def.
    ^* Har ord i noen av navnestrengene/-rutinene som mangler som synonym til objektet.
    ^^";
  style roman;
  anyerr=false;
  objectloop(i>LibraryMessages && ~~(i ofclass class)) {
    err=false;
!    isNormalObj=parent(i)~=0 || i hasnt light;
!    if(isNormalObj && parent(i)==0) { ! Check if dark, lonely objects have exits
!      objectloop(j in compass) isnormalobj=isnormalobj && (i.(j.door_dir)==0);
!    }
    if((specific==0 && _CheckThisObj(i)) || specific==i) {
      if(i.short_name_def==0 && i.short_name==0 && i hasnt proper) {
        err=true;
        print "Mangler b�de short_name_def og short_name: ", (name) i, ".^";
      } else if(i.short_name~=0 && (i.short_name_def~=0 || i.short_name_indef~=0)) {
        err=true;
        print "Har short_name OG short_name_indef eller short_name_def: ", (name) i, ".^";
      }
      if(i.short_name~=0) {
        wordno=_CheckName(i, short_name);
        if(wordno) {
          print "Har ord i short_name som mangler som synonym: ", (name) i, " ",(_PrintBadWords) (wordno),".^";
          err=true;
        }
      }
      if(i.short_name_def~=0) {
        wordno=_CheckName(i, short_name_def);
        if(wordno) {
          print "Har ord i short_name_def som mangler som synonym: ", (name) i, " ",(_PrintBadWords) (wordno),".^";
          err=true;
        }
      }
      if(i.short_name_indef~=0) {
        wordno=_CheckName(i, short_name_indef);
        if(wordno) {
          print "Har ord i short_name_indef som mangler som synonym: ", (name) i, " ",(_PrintBadWords) (wordno),".^";
          err=true;
        }
      }
      if(i.short_name==0 && i.short_name_indef==0) {
        wordno=_CheckName(i);
        if(wordno) {
          print "Har ord i objektets navnestreng som mangler som synonym: ", (name) i, " ",(_PrintBadWords) (wordno),".^";
          err=true;
        }
      }
    }


    if(err) {
      anyerr=true;
    }

  }
  if(~~anyerr) "Ingen problemer funnet.";
];

[CheckGameSelectSub  i;
  style bold;
  print  "Objekter med stjerner rundt navnet unders�kes ikke av falkeblikk:^";
  style roman;
  objectloop(i>LibraryMessages && ~~(i ofclass class)) {
    if(_CheckThisObj(i)) {
      print "(",i,")  ", (name) i, "^";
    } else {
      print "(",i,") *** ", (name) i, " ***^";
    }
  }
];

#endif;

[SetGender x gender;
  if(gender == 0) {
    if(x has male or female or uter or neuter) rtrue;
    if(x has animate) gender = LanguageAnimateGender;
    else gender = LanguageInanimateGender;
  } else {
    give x ~male;
    give x ~female;
    give x ~neuter;
    give x ~uter;
  }
  switch(gender) {
    male: give x male;
    female: give x female;
    uter: give x uter;
    neuter: give x neuter;
  }
];

Class   CompassDirection
  with  number 0,
        description [;
            if (location provides compass_look && location.compass_look(self)) rtrue;
            if (self.compass_look()) rtrue;
            L__M(##Look, 7, self);
        ],
        compass_look false,
  has   scenery proper;
Object Compass "compass" has concealed;
IFNDEF WITHOUT_DIRECTIONS;
CompassDirection -> n_obj "nord"
                    with door_dir n_to, name 'nord' 'n//' 'nordre' 'nordover';
CompassDirection -> s_obj "s�r"
                    with door_dir s_to, name 's�r' 's//' 's�dre' 's�rover' 'syd' 's�ndre';
CompassDirection -> e_obj "�st"
                    with door_dir e_to, name '�st' '�//' '�stre' '�stover';
CompassDirection -> w_obj "vest"
                    with door_dir w_to, name 'vest' 'v//' 'vestre' 'vestover';
CompassDirection -> ne_obj "nord�st"
                    with door_dir ne_to, name 'nord�st' 'n�' 'nord�stre' 'nord�stover';
CompassDirection -> nw_obj "nordvest"
                    with door_dir nw_to, name 'nordvest' 'nv' 'nordvestre' 'nordvestover';
CompassDirection -> se_obj "s�r�st"
                    with door_dir se_to, name 's�r�st' 's�' 's�r�stre' 'syd�st' 'syd�stre' 's�r�stover' 'syd�stover';
CompassDirection -> sw_obj "s�rvest"
                    with door_dir sw_to, name 's�rvest' 'sv' 's�rvestre' 'sydvest' 'sydvestre' 's�rvestover' 'sydvestover';
CompassDirection -> u_obj "opp"
                    with door_dir u_to, name 'opp' 'o//' 'oppover' 'tak' 'taket' 'himmelen' 'himmel' 'luft' 'luften' 'lufta';
CompassDirection -> d_obj "ned"
                    with door_dir d_to, name 'ned' 'nedover' 'gulv' 'gulvet' 'bakke' 'bakken';
ENDIF;
CompassDirection -> in_obj "inn"
                    with door_dir in_to, name 'inn' 'innsiden' 'innover';
CompassDirection -> out_obj "ut"
                    with door_dir out_to, name 'ut' 'utsiden' 'utover';
! ---------------------------------------------------------------------------
!   Part II.   Vocabulary
! ---------------------------------------------------------------------------

! Endringer CustomTense:
! N� er f�lgende variabler globale i stedet for konstanter:
! De gamle konstantene er bortkommenterte for � vise hvor de tidligere var.

Global IS__TX        = " er";
Global ARE__TX       = " er";
Global IS2__TX       = "er ";
Global ARE2__TX      = "er ";

Global YOU__TX       = "Du";
Global LYOU__TX      = "du";
Global FORMER__TX    = "ditt tidigere jeg";
Global YOURSELF__TX  = "deg selv";
Constant MYSELF__TX  = "meg selv";

! Slutt p� endringer for CustomTense:

Constant AGAIN1__WD   = 'igjen';
Constant AGAIN2__WD   = 'g//';
Constant AGAIN3__WD   = 'igjen';
Constant OOPS1__WD    = 'oi';
Constant OOPS2__WD    = 'ups';
Constant OOPS3__WD    = 'oisann';
Constant UNDO1__WD    = 'angre';
Constant UNDO2__WD    = 'angre';
Constant UNDO3__WD    = 'angre';

Constant ALL1__WD     = 'alle';
Constant ALL2__WD     = 'hver';
Constant ALL3__WD     = 'sammen';
Constant ALL4__WD     = 'samtlige';
Constant ALL5__WD     = 'alt';
Constant AND1__WD     = 'og';
Constant AND2__WD     = 'samt';
Constant AND3__WD     = 'ogs�';
Constant BUT1__WD     = 'men ikke';
Constant BUT2__WD     = 'utenom';
Constant BUT3__WD     = 'foruten';
Constant ME1__WD      = 'jeg';
Constant ME2__WD      = 'meg';
Constant ME3__WD      = 'selv';
Constant OF1__WD      = 'av';
Constant OF2__WD      = 'blant';
Constant OF3__WD      = 'utav';
Constant OF4__WD      = 'av';
Constant OTHER1__WD   = 'andre';
Constant OTHER2__WD   = 'annen';
Constant OTHER3__WD   = 'annet';
Constant THEN1__WD    = 's�';
Constant THEN2__WD    = 'deretter';
Constant THEN3__WD    = ',';

Constant NO1__WD      = 'n//';
Constant NO2__WD      = 'nei';
Constant NO3__WD      = 'nei';
Constant YES1__WD     = 'j//';
Constant YES2__WD     = 'ja';
Constant YES3__WD     = 'ok';

Constant AMUSING__WD  = 'morsomme';
Constant FULLSCORE1__WD = 'full score';
Constant FULLSCORE2__WD = 'full';
Constant QUIT1__WD    = 'avslutt';
Constant QUIT2__WD    = 'slutt';
Constant RESTART__WD  = 'omstart';
Constant RESTORE__WD  = 'last';

! �#� The need for uter, in addition to male, female and neuter,
! necessitates the adding of four bits to every GNA-word. While
! there was indeed room for these bits, it still implies that
! one or more of the other library files will have to be modified
! as well to interpret these words correctly in order to handle
! the four genders correctly.

Array LanguagePronouns table ! �#� Usage of this array must be modified to add uter

   !  word       possible GNAs                   connected
   !             to follow:                      to:
   !             a       i
   !             s   p   s   p
   !             mfunmfunmfunmfun

      'han'    $$1000000010000000                NULL
      'henne'  $$0100000001000000                NULL
      'den'    $$0010000000100000                NULL
      'det'    $$0001000000010000                NULL
      'dem'    $$0000111100001111                NULL;
!      'de'    $$0000111100001111                NULL; !�#� Decided to leave this out because it gets ugly when player types "pronomen" (pronouns)

! 'de' n�r pronomenet er subjekt
! 'dem' ved direkte eller indirekte objekt, styring til preposisjon

Array LanguageDescriptors table ! �#� Usage of this array must be modified to add uter

   !  word       possible GNAs    descriptor     connected
   !             to follow:       type:          to:
   !             a       i
   !             s   p   s   p
   !             mfunmfunmfunmfun

      'min'    $$1110000011100000 POSSESS_PK      0
      'mitt'   $$0001000000010000 POSSESS_PK      0
      'mine'   $$0000111100001111 POSSESS_PK      0
! *#* Hard to translate and not very important, skipping
!      'this'   $$111111111111    POSSESS_PK      0
!      'these'  $$000111000111    POSSESS_PK      0
!      'that'   $$111111111111    POSSESS_PK      1
!      'those'  $$000111000111    POSSESS_PK      1
      'hans'   $$1111111111111111 POSSESS_PK      'han'
      'hennes' $$1111111111111111 POSSESS_PK      'henne'
      'deres'  $$1111111111111111 POSSESS_PK      'dem'
      'dens'   $$1111111111111111 POSSESS_PK      'den'
      'dets'   $$1111111111111111 POSSESS_PK      'det'
      'den'    $$1110000011100000 DEFART_PK       NULL ! �#� These have been commented out because their usage
      'det'    $$0001000000010000 DEFART_PK       NULL !     as definite articles conflict with their usage as
      'dem'    $$0000111100001111 DEFART_PK       NULL !     it-words. Should be corrected in the parser somehow.
      'en'     $$1110000011100000 INDEFART_PK     NULL
      'et'     $$0001000000010000 INDEFART_PK     NULL
      'noen'   $$0000111100001111 INDEFART_PK     NULL
      'tent'   $$1111000011110000 light           NULL
      'tente'  $$0000111100001111 light           NULL
    'slukket'  $$1111000011110000 (-light)        NULL
    'slukkede' $$0000111100001111 (-light)        NULL;

Array LanguageNumbers table
    'en' 1 'et' 1 'ei' 1 'to' 2 'par' 2 'tre' 3 'fire' 4 'fem' 5
    'seks' 6 'sju' 7 '�tte' 8 'ni' 9 'ti' 10
    'elleve' 11 'tolv' 12 'tretten' 13 'fjorten' 14 'femten' 15
    'seksten' 16 's�tten' 17 'sytten' 17 'atten' 18 'nitten' 19 'tjue' 20 'tyve' 20;

! ---------------------------------------------------------------------------
!   Part III.   Translation
! ---------------------------------------------------------------------------

[LTI_delete pos n   k m;
!  print pos,",", n, ".";
  if(n<1) n=1;
#ifdef TARGET_ZCODE;
  m=(buffer->1)+3-n;
#ifnot; ! TARGET_GLULX
  m=(buffer-->0)+5-n;
#endif; ! TARGET_
  for(k=pos:k<m:k++)
    buffer->k=buffer->(k+n);
#ifdef TARGET_ZCODE;
  buffer->1=(buffer->1)-n;
#ifnot; ! TARGET_GLULX
  buffer-->0=(buffer-->0)-n;
#endif; ! TARGET_
];

! *#* One could definitely do a lot more work here...

[ LanguageToInformese   i c1 c2 cnew no_transcribe changed printed_first charcount;

! This section handles transciption of aa, ae and oe into �, � and � respectively

#ifdef TARGET_ZCODE;
  if(transcribe_mode && buffer->1>1) {
    for(i=2:i<1+buffer->1:i++) { ! Stop one character before the last
#ifnot; ! TARGET_GLULX
  if(transcribe_mode && buffer-->0>1) {
    for(i=4:i<3+buffer-->0:i++) { ! Stop one character before the last
#endif; ! TARGET_
!      print "#";
      c1=buffer->i;
      c2=buffer->(i+1);
      if(c1=='.')
        break;
      if(c1=='!') {
        no_transcribe++;
        LTI_delete(i);
        i--;
        changed=1;
        continue;
      }

#ifdef TARGET_ZCODE;
      if(c1=='a') {
        if(c2=='a')
          cnew='�';
        else if(c2=='e')
          cnew='�';
      } else if(c1=='o' && c2=='e')
        cnew='�';
#ifnot; ! TARGET_GLULX
      if(c1=='a') {
        if(c2=='a' or 'A')
          cnew='�';
        else if(c2=='e' or 'E')
          cnew='�';
      } else if(c1=='o' && c2=='e' or 'E')
        cnew='�';
      else if(c1=='A') {
        if(c2=='a' or 'A')
          cnew='�';
        else if(c2=='e' or 'E')
          cnew='�';
      } else if(c1=='O' && c2=='e' or 'E')
        cnew='�';
#endif; ! TARGET_

      if(cnew) {
        if(no_transcribe)
          no_transcribe--;
        else {
          buffer->i=cnew;
          LTI_delete(i+1);
          changed=1;
        }
        cnew=0;
      }
    }
    if(changed)
      Tokenise__(buffer,parse);

    if(transcribe_mode>1 && changed) {
      print "(ble transkribert til ~";
      printed_first = false;
#ifdef TARGET_ZCODE;
      charcount = 2+buffer->1;
#ifnot; ! TARGET_GLULX
      charcount = 4+buffer-->0;
#endif; ! TARGET_
      for(i=WORDSIZE:i<charcount:i++) {
        if(printed_first || buffer->i~=' ') {
          printed_first=true;
          if(buffer->i=='.')
            break;
          print (char) buffer->i;
        }
      }

      print "~)";
      new_line;
    }

  }

! This section changes "meg selv" to "meg", to avoid a pronoun consisting of two words

#ifdef TARGET_ZCODE;
  for(i=2:i<=parse->1:i++)
    if(parse-->(2*i-1)=='selv' && parse-->(2*i-3)=='meg') {
      LTI_delete((parse->(4*i+1))-1,(parse->(4*i))+1);
      Tokenise__(buffer,parse);
    }
#ifnot; ! TARGET_GLULX
  for(i=2:i<=parse-->0:i++)
    if(parse-->(3*i-2)=='selv' && parse-->(3*i-5)=='meg') {
      LTI_delete((parse-->(3*i))-1,(parse-->(3*i-1))+1);
      Tokenise__(buffer,parse);
    }
#endif; ! TARGET_

];

[ TranscribeOnSub;
  if(transcribe_mode==0 or 2)
    L__M(##TranscribeOn,2);
  else
    L__M(##TranscribeOn,1);
  transcribe_mode=1;
];

[ TranscribeShowSub;
  if(transcribe_mode==0 or 1)
    L__M(##TranscribeShow,2);
  else
    L__M(##TranscribeShow,1);
  transcribe_mode=2;
];

[ TranscribeOffSub;
  if(transcribe_mode>0)
    L__M(##TranscribeOff,2);
  else
    L__M(##TranscribeOff,1);
  transcribe_mode=0;
];


! ---------------------------------------------------------------------------
!   Part IV.   Printing
! ---------------------------------------------------------------------------

Constant LanguageAnimateGender   = male;
Constant LanguageInanimateGender = uter;

Constant LanguageContractionForms = 1;     ! Norwegian has only 1:
                                           ! 0 = no matter what

[ LanguageContraction text x;
  x=text; ! �#� Only here to avoid a warning
  return 0;
];

Array LanguageArticles -->

 !   Contraction form 0:
 !   Cdef   Def    Indef

! �#� Cdef and Def should not be printed unless there is an adjective before the noun

     "Den " "den " "en "        ! masc/fem/uter sing
     "Det " "det " "et "        ! neuter sing
     "De "  "de "  "noen ";     ! plural

                   !             a               i
                   !             s       p       s       p
                   !             m f u n m f u n m f u n m f u n

Array LanguageGNAsToArticles --> 0 0 0 1 2 2 2 2 0 0 0 1 2 2 2 2; ! �#� Usage of this array must be modified to add uter

[LanguagePrintShortName o  i;
  if(indef_mode==false)
    i=short_name_def;
  else
    i=short_name_indef;
   if(o.&i~=0) {
     if(metaclass(o.i)==String) {
       print (string) o.i;
       rtrue;
     }
     else if(metaclass(o.i)==Routine) {
       if(PrintOrRun(o, i)==true)
         rtrue;
     } else
       rtrue; ! Incorrect type of value
   }
   if(o.&short_name~=0) {
     if(metaclass(o.short_name)==String) {
       print (string) o.short_name;
       rtrue;
     }
     else if(metaclass(o.short_name)==Routine) {
       if(PrintOrRun(o, short_name)==true)
         rtrue;
     } else
       rtrue; ! Incorrect type of value
   }
  print (object) o;
  rtrue;
];

[ LanguageDirection d;
  switch(d)
  {
     n_to: print "nord";
     s_to: print "s�r";
     e_to: print "�st";
     w_to: print "vest";
     ne_to: print "nord�st";
     nw_to: print "nordvest";
     se_to: print "s�r�st";
     sw_to: print "s�rvest";
     u_to: print "ovenfor";
     d_to: print "nedenfor";
     in_to: print "innenfor";
     out_to: print "utenfor";
     default: return RunTimeError(9,d);
  }
];

Constant ENDS_WITH_T = 2;

[ LanguageNumber n force_neuter_1 obj gender f;
  if(obj~=0 && metaclass(obj)==Object) {
    if(obj has neuter) gender=neuter;
    else gender=uter;
  }
  if((gender==neuter || force_neuter_1~=0) && n==1) { print "et"; return ENDS_WITH_T;}
  if (n==0)    { print "null"; rfalse; }
  if (n<0)     { print "minus "; n=-n; }
  if (n>=1000) {
    if(LanguageNumber(n/1000, force_neuter_1, false, neuter)~=ENDS_WITH_T)
      print "t";
    print "usen";
    n=n%1000;
    f=1;
  }
  if (n>=100)  { LanguageNumber(n/100, force_neuter_1, false, neuter); print "hundre"; n=n%100; f=1; }
  if (n==0) rfalse;
  switch(n)
  {   1:  if(force_neuter_1) { print "et"; return ENDS_WITH_T; } else print "en";
      2:  print "to";
      3:  print "tre";
      4:  print "fire";
      5:  print "fem";
      6:  print "seks";
      7:  print "sju";
      8:  print "�tte";
      9:  print "ni";
      10: print "ti";
      11: print "elleve";
      12: print "tolv";
      13: print "tretten";
      14: print "fjorten";
      15: print "femten";
      16: print "seksten";
      17: print "s�tten";
      18: print "atten";
      19: print "nitten";
      20 to 99:
          switch(n/10)
          {  2: print "tjue";
             3: print "tretti";
             4: print "f�rti";
             5: print "femti";
             6: print "seksti";
             7: print "s�tti";
             8: print "�tti";
             9: print "nitti";
          }
          if (n%10 ~= 0) return LanguageNumber(n%10, force_neuter_1);
  }
];

Constant NorwegianNumber LanguageNumber;

[ LanguageTimeOfDay hours mins;
   print hours/10, hours%10, ":", mins/10, mins%10;
];

[ LanguageVerb i;
    switch (i) {
      'e//':            print "eiendeler";
      'k//', 'se':      print "se";
      'u//':            print "unders�k";
      'z//':            print "vent";
      'innspilling':    print "innspilling";
      'transkript':     print "transkript";
      'avspilling':     print "avspilling";
      'a//', 'd�':      print "avslutt";
      't//':            print "ta";
      'j//':            print "ja";
      default: rfalse;
    }
    rtrue;
];

! ----------------------------------------------------------------------------
!  LanguageVerbIsDebugging is called by SearchScope.  It should return true
!  if word w is a debugging verb which needs all objects to be in scope.
! ----------------------------------------------------------------------------

#Ifdef DEBUG;
[ LanguageVerbIsDebugging w;
    if (w == 'purloin' or 'tree' or 'abstract'
                       or 'gonear' or 'scope' or 'showobj')
        rtrue;
    rfalse;
];
#Endif;

! ----------------------------------------------------------------------------
!  LanguageVerbLikesAdverb is called by PrintCommand when printing an UPTO_PE
!  error or an inference message.  Words which are intransitive verbs, i.e.,
!  which require a direction name as an adverb ('walk west'), not a noun
!  ('I only understood you as far as wanting to touch /the/ ground'), should
!  cause the routine to return true.
! ----------------------------------------------------------------------------

[ LanguageVerbLikesAdverb w;
    if (w == 'se' or 'g�' or 'l�p' or 'flytt' or 'trykk' or 'dytt' or 'putt')
        rtrue;
    rfalse;
];

! ----------------------------------------------------------------------------
!  LanguageVerbMayBeName is called by NounDomain when dealing with the
!  player's reply to a "Which do you mean, the short stick or the long
!  stick?" prompt from the parser. If the reply is another verb (for example,
!  LOOK) then then previous ambiguous command is discarded /unless/
!  it is one of these words which could be both a verb /and/ an
!  adjective in a 'name' property.
! ----------------------------------------------------------------------------

[ LanguageVerbMayBeName w;
    if (w == 'full' or 'fulle' or 'lange' or 'normale' or 'korte' or 'vis' or '�pne' or 'v�kne')
        rtrue;
    rfalse;
];


Constant NKEY__TX     = "N = neste emne";
Constant PKEY__TX     = "F = forrige emne";
Constant QKEY1__TX    = "X = tilbake til spillet";
Constant QKEY2__TX    = "X = forrige meny";
Constant RKEY__TX     = "ENTER = les emne";

Constant NKEY1__KY    = 'N';
Constant NKEY2__KY    = 'n';
Constant PKEY1__KY    = 'F';
Constant PKEY2__KY    = 'f';
Constant QKEY1__KY    = 'X';
Constant QKEY2__KY    = 'x';

Constant SCORE__TX    = "Poeng: ";
Constant MOVES__TX    = "Trekk: ";
Constant TIME__TX     = "Tid: ";
Constant CANTGO__TX   = "Du kan ikke g� den veien.";
!Constant FORMER__TX   = "ditt tidligere jeg";
!Constant YOURSELF__TX = "deg selv";
!Constant YOU__TX      = "Du";
Constant DARKNESS__TX = "M�rke";

Constant THOSET__TX   = "dem";
Constant THAT__TX     = "den"; ! �#� Should this be "det" in case we're talking about something neuter?
Constant OR__TX       = " eller ";
Constant NOTHING__TX  = "ingenting";
!Constant IS__TX       = " er";
!Constant ARE__TX      = " er";
!Constant IS2__TX      = "er ";
!Constant ARE2__TX     = "er ";
Constant AND__TX      = " og ";
Constant WHOM__TX     = "som ";
Constant WHICH__TX    = "som ";
Constant COMMA__TX    = ", ";

[ GetSecondaryGNAOfObject obj case gender;
   if (obj hasnt animate) case = 8;

   if (obj has uter) gender = uter;
   if (obj has neuter) gender = neuter;
   if (obj has female) gender = female;
   if (obj has male) gender = male;

   if (gender == 0)
   {   if (case == 0) gender = LanguageAnimateGender;
       else gender = LanguageInanimateGender;
   }
   if (gender == female) case = case + 1;

   if (gender == uter) case = case + 2;
   if (gender == neuter) case = case + 3;
   if (obj has pluralname) case = case + 4;
   return case;
];

[ DenEllerHan obj;   ! Used in the nominative
  SetGender(obj);
  if (obj == player) { print (string) LYOU__TX; return; }
  if (obj has pluralname) { print "de"; return; }
  if (obj has uter) { print "den"; return; }
  if (obj has neuter) { print "det"; return; }
  if (obj has female) { print "hun"; return; }
  print "han";
];
[ CDenEllerHan obj;   ! Used in the accusative
  SetGender(obj);
  if (obj == player) { print (string) YOU__TX; return; }
  if (obj has pluralname) { print "De"; return; }
  if (obj has uter) { print "Den"; return; }
  if (obj has neuter) { print "Det"; return; }
  if (obj has female) { print "Hun"; return; }
  print "Han";
];
[ DenEllerHam obj;   ! Used in the accusative
  SetGender(obj);
  if (obj == player) { print (string) YOURSELF__TX; return; }
  if (obj has pluralname) { print "dem"; return; }
  if (obj has uter) { print "den"; return; }
  if (obj has neuter) { print "det"; return; }
  if (obj has female) { print "henne"; return; }
  print "ham";
];
[ CDenEllerHam obj;   ! Used in the accusative
  SetGender(obj);
  if (obj == player) {
    PrintToBuffer(StorageForShortName, 160, YOURSELF__TX);
    StorageForShortName->WORDSIZE =
        UpperCase(StorageForShortName->WORDSIZE);
    PrintBuffer(StorageForShortName);
    return;
  }
  if (obj has pluralname) { print "Dem"; return; }
  if (obj has uter) { print "Den"; return; }
  if (obj has neuter) { print "Det"; return; }
  if (obj has female) { print "Henne"; return; }
  print "Ham";
];

[ IndefAdjOpen obj;
  if (obj hasnt pluralname) {
    if (obj has neuter) { print "�pent"; return; }
    print "�pen"; return;
  }
  print "�pne";
];
[ IndefAdjClosed obj;
  if (obj hasnt pluralname) {
    if (obj has neuter) { print "lukket"; return; }
    print "lukket"; return;
  }
  print "lukkede";
];
[ IndefAdjEmpty obj;
  if (obj hasnt pluralname) {
    if (obj has neuter) { print "tomt"; return; }
    print "tom"; return;
  }
  print "tomme";
];
[ IndefAdjWorn;
  print "p�";
];
[ IndefAdjLocked obj;
  if (obj hasnt pluralname) { print "l�st"; return; }
  print "l�ste";
];
[ IndefAdjHeld;
  print "holder";
];
[ IndefAdjGiven;
  print "gitt";
];

[ WhichorWhom obj;
  if (obj has pluralname) {print "dem"; return; }
  if (obj has neuter) {print "det"; return; }
  print "den";
];

[ LanguageLM n x1;

  Prompt:  print "^>";

  Answer, Ask:    "Du f�r ikke noe svar.";
  Attack:         "Vold er nok ikke l�sningen her.";
  Blow:           "Det virker meningsl�st � bl�se p� ", (DenEllerHam) x1, ".";
  Burn:           "Det virker lite produktivt, og dessuten farlig.";
  Buy:            "Det er ikke noe � kj�pe.";
  Climb:          "Det virker ikke s�rlig konstruktivt.";
  Close:   switch(n)
           {   1: print_ret (CDenEllerHan) x1,"  er ikke noe du kan lukke.";
               2: print_ret (CDenEllerHan) x1,
                  " er allerede ",(indefadjclosed) x1,".";
               3: "Du lukker ", (the) x1, ".";
           }
  CommandsOff: switch (n) {
        1: "[Kommandoinnspilling avsluttet.]";
        #Ifdef TARGET_GLULX;
        2: "[Kommandoinnspilling var allerede avsluttet.]";
        #Endif; ! TARGET_
    }
  CommandsOn: switch (n) {
        1: "[Kommandoinnspilling startet.]";
        #Ifdef TARGET_GLULX;
        2: "[Kommandoer spilles n� inn.]";
        3: "[Kommandoinnspilling var allerede startet.]";
        4: "[Kommandoinnspilling feilet.]";
        #Endif; ! TARGET_
    }
  CommandsRead: switch (n) {
        1: "[Spiller av kommandoer.]";
        #Ifdef TARGET_GLULX;
        2: "[Kommandoer spilles allerede av.]";
        3: "[Avspilling av kommandoer feilet. Kommandoinnspilling p�g�r.]";
        4: "[Avspilling av kommandoer feilet.]";
        5: "[Avspilling av kommandoer avsluttet.]";
        #Endif; ! TARGET_
    }
  Consult:        "Du oppdager ingenting interessant i ", (the) x1, ".";
  Cut:            "� skj�re i ", (DenEllerHam) x1, " virker ikke s�rlig konstruktivt.";
  Dig:            "Det har ikke noen hensikt � grave her.";
  Disrobe: switch(n)
           {   1: "Du har ", (CDenEllerHam) x1, " ikke p� deg.";
               2: "Du tar av ", (the) x1, ".";
           }
  Drink:          "Det finnes ingenting som er verdt � drikke her.";
  Drop:    switch(n)
           {   1: print_ret (CDenEllerHan) x1, " er her allerede.";
               2: "Du har ", (DenEllerHam) x1, " ikke.";
               3: "(tar f�rst av ", (the) x1, ")";
               4: "Sluppet.";
           }
  Eat:     switch(n)
           {   1: print (CDenEllerHan) x1," er uspiselig";
                  if(x1 has pluralname) "e.";
                  else ".";
               2: "Du spiser ", (the) x1, ". Ikke s� verst.";
           }
  EmptyT:  switch(n)
           {   1: print_ret (The) x1, " kan ikke inneholde noe.";
               2: print_ret (The) x1, " er ",(indefadjclosed) x1,".";
               3: print_ret (The) x1, " er allerede ",(indefadjempty) x1,".";
               4: "Det har ingen hensikt.";
           }
  Enter:   switch(n)
           {   1: print "Men du er jo allerede ";
                  if (x1 has supporter) print "p� "; else print "i ";
                  print_ret (the) x1, ".";
               2: print (CDenEllerHan) x1, " er ikke egnet til "; ! �#� Should be more varied.
                  switch (verb_word) {
                      'st�', 'still': "� st� p�.";
                      'sitt', 'sett': "� sitte p�.";
                      'ligg', 'legg': "� ligge p�.";
                      default: "det.";
                  }
               3: print_ret (The) x1, " er ",(indefadjclosed) x1,", s� det blir vanskelig.";
               4: "Du m� st� p� egne ben f�rst.";
               5: switch (verb_word) {
                      'st�','still': print "Du stiller deg";
                      'sitt','sett': print "Du setter deg";
                      'ligg','legg': print "Du legger deg";
                      default: print "Du klyver "; if(x1 has supporter) print "opp"; else print "inn";
                  }
                  if (x1 has supporter) print " p� "; else print " i ";
                  print_ret (the) x1, ".";
               6: "(du forlater ",(the) x1, ")";
               7: if (x1 has supporter) "(klyver opp p� ", (the) x1, ")^";
                  if (x1 has container) "(klyver inn i ", (the) x1, ")^";
                  "(g�r p� ", (the) x1, ")^";
           }
  Examine: switch(n)
           {   1: "M�rke -t (norr. myrkr). M�rk tilstand, mangel p� lys (� se i).";
               2: "Du ser ser ikke noe spesielt ved ", (the) x1, ".";
               3: print (The) x1, " er ";
                  if (x1 has on) "p�."; else "av.";
           }
  Exit:    switch(n)
           {   1: "Men du er ikke i noe som helst.";
               2: "Det blir vanskelig, siden ", (the) x1, " er ",(indefadjclosed) x1,".";
               3: "Du forlater ",(the) x1, ".";
               4: print "Men du er ikke ";
                  if (x1 has supporter) print "p� "; else print "i ";
                  print_ret (the) x1, ".";
           }
  Fill:           "Men det finnes ikke noe � fylle opp med her.";
  FullScore: switch(n)
           {   1: if (deadflag) print "Du fikk ";
                  else          print "Du har ";
                  "poeng, fordelt p�:^";
               2: "finne viktige ting";
               3: "bes�ke viktige steder";
               4: print "totalt (av ", MAX_SCORE; ")";
           }
  GetOff:         "Men du er ikke p� ", (the) x1, " for �yeblikket.";
  Give:    switch(n)
           {   1: "Du har ikke ", (the) x1, ".";
               2: "Du holder ", (the) x1,
                  " ut foran deg, men det skjer ikke stort.";
               3: print_ret (The) x1, " virker ikke � bry seg om tilbudet.";
           }
  Go:      switch(n)
           {   1: print "F�rst m� du klatre ";
                  if (x1 has supporter) print "ned fra "; else print "ut av ";
                  print_ret (the) x1, ".";
               2: print_ret (string) CANTGO__TX;   ! "You can't go that way."
               3: "Du kan ikke klatre opp p� ", (the) x1, ".";
               4: "Du kan ikke klatre ned p� ", (the) x1, ".";
               5: "Det blir vanskelig, ", (the) x1, " er i veien.";
               6: "Det kan du ikke, ", (the) x1," leder ingen steder.";
           }
  Insert:  switch(n)
           {   1: "Du m� ha ", (the) x1,
                  " innan du kan legge ", (DenEllerHan) x1,
                  " i noe.";
               2: print_ret (CDenEllerHan) x1, " kan ikke inneholde noe.";
               3: print_ret (The) x1, " er ",(indefadjclosed) x1,".";
               4: "Du m� ta ", (DenEllerHam) x1, " av f�rst.";
               5: "Rekursjon -en. Se 'rekursjon'.";
               6: "(tar ", (DenEllerHam) x1, " f�rst av)^";
               7: "Det er ikke plass til noe mer i ", (the) x1, ".";
               8: "Utf�rt.";
               9: "Du plasserer ", (the) x1, " i ", (the) second, ".";
           }
  Inv:     switch(n)
           {   1: "Du har ingenting.";
               2: print "Du har";
               3: print ":^";
               4: print ".^";
           }
  Jump:           "Du hopper litt opp og ned. Det gir deg ikke noe s�rlig.";
  JumpOver, Tie:  "Det ville ikke v�re til noe s�rlig hjelp.";
  Kiss:           "Du dr�mmer deg bort et �yeblikk.";
  Listen:         "Du h�rer ikke noe uventet.";
  ListMiscellany:
           switch(n)
           {   1: print " (lyser)";
               2: print " (",(indefadjclosed) x1,")";
               3: print " (som er ",(indefadjclosed) x1," og lyser)";
               4: print " (",(indefadjempty) x1,")";
               5: print " (som er ",(indefadjempty) x1," og lyser)";
               6: print " (som er ",(indefadjclosed) x1," og ",(indefadjempty) x1,")";
               7: print " (som er ",(indefadjclosed) x1,", ",(indefadjempty) x1," og lyser)";
               8: print " (som lyser og er ",(indefadjworn) x1;
               9: print " (lyser";
              10: print " (",(indefadjworn) x1;
              11: print " (som ";
              12: print "er ", (indefadjopen) x1;
              13: print "er ", (indefadjopen) x1," men ",(indefadjempty) x1;
              14: print "er ", (indefadjclosed) x1;
              15: print "er ", (indefadjclosed) x1," og ",(indefadjlocked) x1;
              16: print " og ",(indefadjempty) x1;
              17: print " (som er ",(indefadjempty) x1,")";
              18: print " inneholder ";
              19: print " (p� ";
              20: print ", opp� ";
              21: print " (i ";
              22: print ", inni ";
           }
  LMode1:         " er n� i ~normal~ utskriftsmodus, som gir lange beskrivelser av steder du bes�ker for f�rste gang, og ellers korte beskrivelser.";
  LMode2:         " er n� i ~lang~ utskriftsmodus, som alltid gir lange beskrivelser av steder (selv om du har bes�kt dem tidligere).";
  LMode3:         " er n� i ~kort~ utskriftsmodus, som alltid gir korte beskrivelser av steder (selv om du bes�ker dem for f�rste gang).";
  Lock:    switch(n)
           {   1: print_ret (CDenEllerHan) x1, " virker ikke � v�re noe du kan l�se.";
               2: print_ret (CDenEllerHan) x1, " er allerede ",(indefadjlocked) x1,".";
               3: "Du m� f�rst lukke ", (the) x1, ".";
               4: print_ret (CDenEllerHan) x1, " virker ikke som ", (CDenEllerHan) x1, " passer l�sen.";
               5:  "Du l�ser ", (the) x1, ".";
           }
  Look:    switch(n)
           {   1: print " (p� ", (the) x1, ")";
               2: print " (i ", (the) x1, ")";
               3: print " (som ", (object) x1, ")";
               4: print "^P� ", (the) x1;
                  WriteListFrom(child(x1),
                    ENGLISH_BIT+RECURSE_BIT+PARTINV_BIT+TERSE_BIT+CONCEAL_BIT+ISARE_BIT);
                  ".";
               5,6:
                   if (x1 ~= location) {
                       if (x1 has supporter) print "^P� "; else print "^I ";
                       print (the) x1, " kan du";
                   }
                   else print "^Du kan";
                   if (n==5) print " ogs�";
                   print " se ";
                   WriteListFrom(child(x1),
                     ENGLISH_BIT+RECURSE_BIT+PARTINV_BIT+TERSE_BIT+CONCEAL_BIT+WORKFLAG_BIT);
                   if (x1 ~= location) ".";
                   " her.";
               7:  "Du ser ikke noe spesielt i den retningen.";
           }
  LookUnder: switch(n)
           {   1: "Det er veldig m�rkt.";
               2: "Du finner ingenting av interesse.";
           }
  Mild:           "Akkurat det jeg ogs� tenkte.";
  Miscellany:
           switch(n)
           {   1: "(tar bare hensyn til de f�rste seksten tingene)^";
               2: "Ingenting � gj�re!";
               3: print " Du er d�d ";
               4: print " Du har vunnet ";
               5: print "^Vil du ha OMSTART, LASTE et lagret spill";
                  #IFDEF DEATH_MENTION_UNDO;
                  print ", ANGRE p� ditt siste trekk";
                  #ENDIF;
                  if (TASKS_PROVIDED==0)
                      print ", vise FULL informasjon om poengene dine";
                  if (deadflag==2 && AMUSING_PROVIDED==0)
                      print ", se noen forslag p� MORSOMME ting � pr�ve ut";
                  " eller AVSLUTTE?";
               6: "[Spilltolkeren st�tter desverre ikke ~angre~.]";
               7: "~Angre~ feilet. [Ikke alle spilltolker st�tter ~angre~.]";
               8: "Velg et av svarene over.";
               9: "^N� er det bekm�rkt her.";
              10: "Unnskyld?";
              11: "[Du kan ikke angre p� noe du ikke har gjort.]";
              12: "[Du kan dessverre ikke angre to ganger p� rad.]";
              13: "[Siste trekk angret.]";
              14: "Det kan dessverre ikke rettes opp.";
              15: "Det var da s� lite.";
              16: "~Oi~ kan bare rette opp ett enkelt ord.";
              17: "Det er bekm�rkt, og du kan ikke se noe som helst.";
              18: print "deg selv";
              19: "Ser like bra ut som alltid.";
              20: "For � gjenta en kommando som ~frosk, hopp~, bare skriv
                   ~igjen~, ikke ~frosk, igjen~.";
              21: "Det fungerer nok ikke � gj�re det igjen.";
              22: "Du kan ikke starte en ytring med et komma.";
              23: "Det virker som du prate med noen, men jeg forst�r ikke med hvem.";
              24: "Du kan ikke prate med ", (the) x1, ".";
              25: "For � prate med noen, pr�v ~person, hei~ eller noe liknende.";
              26: "(tar f�rst opp ", (the) not_holding, ")";
              27: "Den ytringen forsto jeg ikke.";
              28: print "Jeg tror jeg forsto ~";
              29: "Det tallet forsto jeg ikke.";
              30: "Du ser ikke noe s�nt her.";
              31: "Det virker som du har sagt for lite.";
              32: "Du har ikke ", (CDenEllerHam) x1, "."; ! �#� Could be rephrased to include the correct Den/Det, using ThatorThose?
              33: "Du kan ikke bruke det verbet p� flere ting.";
              34: "Du kan du kan bare nevne flere ting �n gang per linje.";
              35: "Jeg er ikke sikker p� hva ~", (address) pronoun_word,
                  "~ refererer til.";
              36: "Du unntok noe som allikevel ikke var med.";
              37: "Du kan ikke gj�re det med noe levende.";
              38: "Jeg kjenner ikke til det verbet.";
              39: "Det er ikke noe du har bruk for i dette spillet.";
              40: "Du kan ikke se ~", (address) pronoun_word,
                  "~ (", (the) pronoun_obj, ") akkurat n�.";
              41: "Jeg forsto ikke riktig den avslutningen.";
              42: print "Det finnes ";
                  if (x1==0) print "ingen";
                  else { print "bare "; LanguageNumber(x1, 0, multiple_object-->1); }
                  if (x1==1) {
                    if(multiple_object-->1 has neuter)
                      " slikt tilgjengelig.";
                    " slik tilgjengelig.";
                  } else
                    " slike tilgjengelige."; ! �#� Tries to care about gender
              43: "Ingenting � gj�re.";
              44: switch(action_to_be) {
                    ##Drop, ##Insert, ##PutOn: "Du har ingenting � slippe.";
                    default: "Det er ingenting � ta.";
                  }
              45: print "Hva mener du, ";
              46: print "Mener du ";
              47: "Du kan bare angi en ting her, n�yaktig hvilken mener du?";
              48: print "Jeg antar at du mener ~"; ! �#� This message should work now
                  if (actor~=player) print (name) actor, ", ";
                  PrintCommand();
                  style bold;
                  print " noen";
                  style roman;
                  print "~, men hvem?^";
              49: print "Jeg antar at du mener ~";
                  if (actor~=player) print (the) actor, ", ";
                  PrintCommand();
                  style bold;
                  print " noe";
                  style roman;
                  print "~, men hva?^";
              50: print "Du ";
                  if (x1 > 0) print "fikk"; else { x1 = -x1; print "mistet"; }
                  print " akkurat ";
                  LanguageNumber(x1,false,false,neuter);
                  print " poeng";
              51: "(ettersom noe dramatisk har skjedd, har din serie med kommandoer blitt avbrutt)";
              52: "^Skriv et tall mellom 1 og ", x1,
                  ", 0 for � vise dette igjen, eller trykk ENTER.";
              53: "^[Trykk MELLOMROM.]";
              54: "[Kommentar lagret.]";
              55: "[Kommentar ikke lagret.]";
              56: "~, men etter det mistet du meg.";
              57: print "?^";
           }
  NotifyOn:       "Poengbeskjeder p�.";
  NotifyOff:      "Poengbeskjeder av.";
  Objects: switch(n)
           {   1: "Objekter du har benyttet:^";
               2: "Ingen.";
               3: print "   (",(indefadjworn) x1,")"; ! �#� Is x1 actually set to the object# in question here? (It is now)
               4: print "   (",(indefadjheld) x1,")"; ! �#� Is x1 actually set to the object# in question here? (It is now)
               5: print "   (",(indefadjgiven) x1,")"; ! �#� Is x1 actually set to the object# in question here? (It is now)
               6: print "   (i ", (name) x1, ")";
               7: print "   (i ", (the) x1, ")";
               8: print "   (inni ", (the) x1, ")";
               9: print "   (p� ", (the) x1, ")";
              10: print "   (borte)";
           }
  Open:    switch(n)
           {   1: print_ret (CDenEllerHan) x1,
                  " kan ikke �pnes.";
               2: print_ret (CDenEllerHan) x1," virker ",(indefadjlocked) x1,".";
               3: print_ret (CDenEllerHan) x1," er allerede ",(indefadjopen) x1,".";
               4: print "Du �pner ", (the) x1, " og finner ";
                  if (WriteListFrom(child(x1),
                      ENGLISH_BIT + TERSE_BIT + CONCEAL_BIT)==0) "ingenting.";
                  ".";
               5: "Du �pner ", (the) x1, ".";
           }
  Order:          print_ret (The) x1," har viktigere ting � gj�re.";
  Places: switch (n) {
        1:  print "Steder du har bes�kt:^";
        2:  print ".^";
    }
  Pray:           "Det skjer ikke noe spesielt.";
  Pronouns: switch(n)
           {   1: print "Akkurat n� refererer ";
               2: print "til ";
               3: print "ikke til noe spesielt";
               4: "ingen pronomen til noe som helst.";
               5: ".";
           }
  Pull, Push, Turn:
           switch(n)
           {   1: print_ret (CDenEllerHan) x1, " sitter fast.";
               2: "Det g�r visst ikke.";
               3: "Ingenting ser ut til � skje.";
               4: "Det ville v�re direkte uh�flig.";
           }
  PushDir: switch(n)
           {   1: "Er det det beste du kommer p�?";
               2: "Det er ikke noen retning.";
               3: "Det g�r nok ikke i den retningen.";
           }
  PutOn:   switch(n)
           {   1: "Du m� holde ", (the) x1,
                  " f�r du kan legge ", (DenEllerHam) x1,
                  " p� noe.";
               2: "Du kan ikke legge noe p� seg selv.";
               3: "� legge ting p� ", (the) x1, " leder ingensteder.";
               4: "Du er ikke smidig nok.";
               5: "(tar ", (DenEllerHam) x1, " f�rst av)^";
               6: "Det er ikke plass til noe mer p� ", (the) x1, ".";
               7: "Utf�rt.";
               8: "Du plasserer ", (the) x1, " p� ", (the) second, ".";
           }
  Quit:    switch(n)
           {   1: print "Svar ja eller nei.";
               2: print "er du sikker p� at du vil avslutte? ";
           }
  Remove:  switch(n)
           {   1: print_ret (CDenEllerHan) x1, " er dessverre ",(indefadjclosed) x1, ".";
               2: print_ret (DenEllerHan) x1, " er ikke der n�.";
               3: "Fjernet.";
           }
  Restart: switch(n)
           {   1: print "er du sikker p� at du vil starte p� nytt? ";
               2: "Omstart feilet.";
           }
  Restore: switch(n)
           {   1: "Lasting av spill feilet.";
               2: "Ok.";
           }
  Rub:            "Det gir ikke noe s�rlig resultat.";
  Save:    switch(n)
           {   1: "Lagring av spill feilet.";
               2: "Ok.";
           }
  Score:   switch (n) {
               1: if (deadflag) print "I dette spillet fikk du ";
                  else print "Du har hittil f�tt ";
                  print score, " poeng av ", MAX_SCORE,
                  " mulige p� ", turns, " trekk";
                  return;
               2: "Det er ingen poeng i dette spillet.";
           }
  ScriptOn: switch(n)
           {   1: "Utskrift er allerede p�.";
               2: "Her starter en utskrift av";
               3: "Start av utskrift feilet.";
           }
  ScriptOff: switch(n)
           {   1: "Utskrift er allerede av.";
               2: "^Her slutter utskriften.";
               3: "Stopp av utskrift feilet.";
           }
  Search:  switch(n)
           {   1: "Men det er helt m�rkt.";
               2: "Det er ingenting p� ", (the) x1, ".";
               3: print "P� ", (the) x1;
                  WriteListFrom(child(x1),
                      TERSE_BIT + ENGLISH_BIT + ISARE_BIT + CONCEAL_BIT);
                  ".";
               4: "Du finner ingenting av interesse.";
               5: "Du ser ikke hva som er i ", (the) x1, ", siden ",
                  (DenEllerHan) x1, " er ",(indefadjclosed) x1,".";
               6: print_ret (The) x1, " er ",(indefadjempty) x1,".";
               7: print "I ", (the) x1;
                  WriteListFrom(child(x1),
                      TERSE_BIT + ENGLISH_BIT + ISARE_BIT + CONCEAL_BIT);
                  ".";
           }
  Set:            "Nei, du kan ikke sette ", (DenEllerHam) x1, ".";
  SetTo:          "Nei, du kan ikke sette ", (DenEllerHam) x1, " til noe.";
  Show:    switch(n)
           {   1: "Du har ikke ", (the) x1, ".";
               2: print (The) x1, " er ikke imponer";
                  if (x1 has pluralname) "te.";
                  else "t.";
           }
  Sing:           "Du har ikke mye til sangstemme.";
  Sleep:          "Du f�ler deg ikke s�rlig s�vnig.";
  Smell:          "Du kjenner ingen uventede lukter.";
  Sorry:          "Det er greit. For denne gangen.";
  Squeeze: switch(n)
           {   1: "Ikke pell!";
               2: "Det gir ikke noe merkbart resultat.";
           }
  Strong:         "Ekte eventyrere snakker ikke s�nn.";
  Swim:           "Det er ingenting � sv�mme i.";
  Swing:          "Det er ingenting fornuftig � svinge p� her.";
  SwitchOn: switch(n)
           {   1: print_ret (CDenEllerHan) x1," er ikke noe du kan skru av eller p�.";
               2: print_ret (CDenEllerHan) x1,
                  " er allerede p�.";
               3: "Du sl�r p� ", (the) x1, ".";
           }
  SwitchOff: switch(n)
           {   1: print_ret (CDenEllerHan) x1," er ikke noe du kan skru av eller p�.";
               2: print_ret (CDenEllerHan) x1,
                  " er allerede av.";
               3: "Du sl�r av ", (the) x1, ".";
           }
  Take:    switch(n)
           {   1: print "Tatt.^";
               2: "Du er s� selvopptatt.";
               3: "Det vil ",(DenEllerHan) x1," nok ikke.";
               4: print_ret "Da m� du f�rst forlate ",(the) x1, ".";
               5: "Du har ", (DenEllerHam) x1, " allerede.";
               6: print_ret (CDenEllerHan) noun, " ser ut til � tilh�re ", (the) x1, ".";
               7: print_ret (CDenEllerHan) noun, " ser ut til � v�re en del av ", (the) x1, ".";
               8: print_ret "Du kommer ikke til ",(DenEllerHam) x1, ".";
               9: print_ret (The) x1, " er ikke ",(indefadjopen) x1, ".";
              10: print_ret "Du kan neppe flytte ", (DenEllerHam) x1, ".";
              11: print_ret (CDenEllerHan) x1, " sitter fast.";
              12: "Du b�rer allerede p� for mye.";
              13: "(legger ", (the) x1, " i ", (the) SACK_OBJECT,
                  " for � f� plass)";
           }
  Taste:          "Du merker ingen uventet smak.";
  Tell:    switch(n)
           {   1: "Du sm�prater litt med deg selv.";
               2: "Ingen reaksjon.";
           }
  Think:          "Du grubler litt.";
  ThrowAt: switch(n)
           {   1: "Meningsl�st.";
               2: "N�r det kommer til stykket t�r du ikke.";
           }
  Touch:   switch(n)
           {   1: "Bort med labbene!";
               2: "Det f�les omtrent som forventet.";
               3: "Hvis du tror det hjelper, v�r s� god.";
           }
  TranscribeOn:
           switch(n)
           {   1: "Transkripsjonsmodus er allerede skrudd p�.";
               2: "Transkripsjonsmodus skrudd p�. ~aa~ betyr n� �, ~ae~ betyr �
                   og ~oe~ betyr �. Skriv et utropstegn f�r hver slik
                   bokstavkombinasjon som ikke skal transkriberes, som feks. i ~!aapne skapet~.
                   Skriv ~transkripsjon vis~ for � f� beskjed n�r transkripsjon blir brukt.";
           }
  TranscribeOff:
           switch(n)
           {   1: "Transkripsjonsmodus er allerede skrudd av.";
               2: "Transkripsjonsmodus skrudd av.";
           }
  TranscribeShow:
           switch(n)
           {   1: "Transkripsjonsmodus med visning er allerede skrudd p�.";
               2: "Transkripsjonsmodus med visning skrudd p�. ~aa~ betyr n� �, ~ae~ betyr �
                   og ~oe~ betyr �. Skriv et utropstegn f�r hver slik
                   bokstavkombinasjon som ikke skal transkriberes, som feks. i ~!aapne skapet~.";
           }
  Unlock:  switch(n)
           {   1: print_ret (CDenEllerHan) x1, " ser ikke ut til � v�re noe du kan l�se opp.";
               2: print_ret (CDenEllerHan) x1," er allerede u",(indefadjlocked) x1,".";
               3: print_ret (CDenEllerHan) x1, " ser ikke ut til � passe til l�sen.";
               4: "Du l�ser opp ", (the) x1, ".";
           }
  VagueGo:       "Du m� oppgi hvilken retning du vil g�.";
  Verify:  switch(n)
           {   1: "Spillfilen ble testet, og ser ut til � v�re inntakt.";
               2: "Spillfilen ble testet, men ser ikke ut til � v�re inntakt.
                   Det er noe alvorlig galt med den.";
           }
  Wait:           "Du venter.";
  Wake:           "Dette er dessverre ingen dr�m.";
  WakeOther:      "Det virker un�dvendig.";
  Wave:    switch(n)
           {   1: "Du har ", (DenEllerHan) x1, " ikke.";
               2: "Du ser litt dum ut der du vifter med ", (the) x1, ".";
           }
  WaveHands:      "Du veiver med armene, og f�ler deg litt dum.";
  Wear:    switch(n)
           {   1: "Du kan ikke ta p� deg ", (DenEllerHam) x1, ".";
               2: "Du har ", (DenEllerHam) x1, " ikke.";
               3: print_ret (CDenEllerHan) x1, " har du allerede p� deg.";
               4: "Du tar p� deg ", (the) x1, ".";
           }
  Yes, No:        "Det var et retorisk sp�rsm�l.";
];

! ==============================================================================

Constant LIBRARY_NORWEGIAN;       ! for dependency checking.

! ==============================================================================
