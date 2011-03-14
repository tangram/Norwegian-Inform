! This translation has a license which should always be distributed
! along with it. It basically says that you're free to use the translation,
! as long as you don't sell it and don't claim that you've written it.
! You may sell games compiled using this translation though.

! ####################################################
! Replacements for routines in Parserm.h
! ####################################################

! All changes are preceded by ¤#¤ in the source code.

! This file replaces the following routines in parserm.h :
!
! ResetDescriptors                          FIXED 6/11
! Descriptors           den/den             FIXED 6/11
! ScoreMatchL                               FIXED 6/11
! PrintCommand                              FIXED 6/11
! CantSee                                   FIXED 6/11
! GetGNAOfObject                            FIXED 6/11
! PronounNotice                             FIXED 6/11
! PrefaceByArticle                          FIXED 6/11
! Indefart                                  FIXED 6/11
! CInDefart                                 FIXED 6/11
! Defart                                    FIXED 6/11
! CDefart                                   FIXED 6/11
!
! Almost all these routines are replaced because they deal with the three genders, which
! had to be increased to four genders in order to make Inform understand Swedish/Norwegian.
!
! ResetDescriptors and ScoreMatchL deals with indef_cases, whose functions has changed.
! ScoreMatchL also deals with PowersOfTwo_TB which has been replaced by PowersOfTwo_GNA
!
! PrintCommand printed the name of the player object in 2nd person when it should be in
! 1st person.
!
! CantSee needed a fix to stop "ta på meg jkjkl" -> "You can't see me(yourself) here."
!
! GetGNAOfObject has to be changed because it calculates the GNA of an object.
!
! Indefart, CInDefart, Defart and CDefart had to be rewritten completely to
! work with Swedish/Norwegian articles
!
! All other routines use at least one of these tables/arrays (which have been modified):
! LanguagePronouns, LanguageDescriptors, LanguageGNAsToArticles
!
! Descriptors also had to be modified to be clever about the duality of "den" as both
! an it-word and a descriptor.

! ####################################################
! Replacements for routines in Verblibm.h
! ####################################################
!
! The following routines in verblibm.h are replaced :
!
! WriteAfterEntry                           FIXED 6/11
! Objects1Sub                               FIXED 6/11

#ifndef LIBRARY_NORWEGIAN;
  #ifdef LIBRARY_ENGLISH;
    Message "ERROR: You must use 'inform +language_name=Norwegian ...' to compile.";
  #ifnot;
    Message "ERROR: Files MUST be included in this order: Parser, Verblib, NorRout, NorGram.";
    abort;
  #endif;
#endif;

Array PowersOfTwo_GNA                 ! Used in converting GNA numbers
  --> $$1000000000000000              ! to bit masks. Used instead of
      $$0100000000000000              ! PowersOfTwo_BT.
      $$0010000000000000
      $$0001000000000000
      $$0000100000000000
      $$0000010000000000
      $$0000001000000000
      $$0000000100000000
      $$0000000010000000
      $$0000000001000000
      $$0000000000100000
      $$0000000000010000
      $$0000000000001000
      $$0000000000000100
      $$0000000000000010
      $$0000000000000001;



! ==============================================================================================================
! Uses indef_cases, whose function has changed
! ==============================================================================================================


[ ResetDescriptors;
   indef_mode=0; indef_type=0; indef_wanted=0; indef_guess_p=0;
   indef_possambig = false;
   indef_owner = nothing;
! ¤#¤ Changed one line
!   indef_cases = $$111111111111;
   indef_cases = $$1111111111111111;
   indef_nspec_at = 0;
];

! ==============================================================================================================
! Uses LanguageDescriptors - However, modification seems unnecessary
! ==============================================================================================================


[ Descriptors  o x flag cto type n;
    ResetDescriptors();
    if (wn > num_words) return 0;

    for (flag=true : flag :) {
        o = NextWordStopped(); flag = false;

! ¤#¤ Added 7 lines to stop "ta den" to be interpreted as "take the"
      if(wn==num_words + 1) {
        for(x=1:x<=LanguagePronouns-->0:x++)
          if(o==LanguagePronouns-->x) {
            wn--;
            return 0;
          }
      }

       for (x=1 : x<=LanguageDescriptors-->0 : x=x+4)
            if (o == LanguageDescriptors-->x) {
                flag = true;
                type = LanguageDescriptors-->(x+2);
                if (type ~= DEFART_PK) indef_mode = true;
                indef_possambig = true;
                indef_cases = indef_cases & (LanguageDescriptors-->(x+1));

                if (type == POSSESS_PK) {
                    cto = LanguageDescriptors-->(x+3);
                    switch (cto) {
                      0: indef_type = indef_type | MY_BIT;
                      1: indef_type = indef_type | THAT_BIT;
                      default:
                        indef_owner = PronounValue(cto);
                        if (indef_owner == NULL) indef_owner = InformParser;
                    }
                }

                if (type == light)  indef_type = indef_type | LIT_BIT;
                if (type == -light) indef_type = indef_type | UNLIT_BIT;
            }

        if (o == OTHER1__WD or OTHER2__WD or OTHER3__WD) {
            indef_mode = 1; flag = 1;
            indef_type = indef_type | OTHER_BIT;
        }
        if (o == ALL1__WD or ALL2__WD or ALL3__WD or ALL4__WD or ALL5__WD) {
            indef_mode = 1; flag = 1; indef_wanted = 100;
            if (take_all_rule == 1) take_all_rule = 2;
            indef_type = indef_type | PLURAL_BIT;
        }
        if (allow_plurals) {
            n = TryNumber(wn-1);
            if (n == 1) { indef_mode = 1; flag = 1; }
            if (n > 1) {
                indef_guess_p = 1;
                indef_mode = 1; flag = 1; indef_wanted = n;
                indef_nspec_at = wn-1;
                indef_type = indef_type | PLURAL_BIT;
            }
        }
        if (flag == 1 && NextWordStopped() ~= OF1__WD or OF2__WD or OF3__WD or OF4__WD)
            wn--;  ! Skip 'of' after these
    }
    wn--;
    return 0;
];



! ==============================================================================================================
! Uses indef_cases and PowersOfTwo_TB - Has been changed
! ==============================================================================================================

! ----------------------------------------------------------------------------
!  ScoreMatchL  scores the match list for quality in terms of what the
!  player has vaguely asked for.  Points are awarded for conforming with
!  requirements like "my", and so on.  Remove from the match list any
!  entries which fail the basic requirements of the descriptors.
! ----------------------------------------------------------------------------

[ ScoreMatchL context its_owner its_score obj i j threshold met a_s l_s;
!   if (indef_type & OTHER_BIT ~= 0) threshold++;
    if (indef_type & MY_BIT ~= 0)    threshold++;
    if (indef_type & THAT_BIT ~= 0)  threshold++;
    if (indef_type & LIT_BIT ~= 0)   threshold++;
    if (indef_type & UNLIT_BIT ~= 0) threshold++;
    if (indef_owner ~= nothing)      threshold++;

    #Ifdef DEBUG;
    if (parser_trace >= 4) print "   Scoring match list: indef mode ", indef_mode, " type ",
      indef_type, ", satisfying ", threshold, " requirements:^";
    #Endif; ! DEBUG

    a_s = SCORE__NEXTBESTLOC; l_s = SCORE__BESTLOC;
    if (context == HELD_TOKEN or MULTIHELD_TOKEN or MULTIEXCEPT_TOKEN) {
        a_s = SCORE__BESTLOC; l_s = SCORE__NEXTBESTLOC;
    }

    for (i=0 : i<number_matched : i++) {
        obj = match_list-->i; its_owner = parent(obj); its_score=0; met=0;

        !      if (indef_type & OTHER_BIT ~= 0
        !          &&  obj ~= itobj or himobj or herobj) met++;
        if (indef_type & MY_BIT ~= 0 && its_owner == actor) met++;
        if (indef_type & THAT_BIT ~= 0 && its_owner == actors_location) met++;
        if (indef_type & LIT_BIT ~= 0 && obj has light) met++;
        if (indef_type & UNLIT_BIT ~= 0 && obj hasnt light) met++;
        if (indef_owner ~= 0 && its_owner == indef_owner) met++;

        if (met < threshold) {
            #Ifdef DEBUG;
            if (parser_trace >= 4) print "   ", (The) match_list-->i, " (", match_list-->i, ") in ",
              (the) its_owner, " is rejected (doesn't match descriptors)^";
            #Endif; ! DEBUG
            match_list-->i = -1;
        }
        else {
            its_score = 0;
            if (obj hasnt concealed) its_score = SCORE__UNCONCEALED;

            if (its_owner == actor) its_score = its_score + a_s;
            else
                if (its_owner == actors_location) its_score = its_score + l_s;
                else
                    if (its_owner ~= compass) its_score = its_score + SCORE__NOTCOMPASS;

            its_score = its_score + SCORE__CHOOSEOBJ * ChooseObjects(obj, 2);

            if (obj hasnt scenery) its_score = its_score + SCORE__NOTSCENERY;
            if (obj ~= actor) its_score = its_score + SCORE__NOTACTOR;

            !   A small bonus for having the correct GNA,
            !   for sorting out ambiguous articles and the like.

! ¤#¤ Changed one line
!          if (indef_cases & (PowersOfTwo_TB-->(GetGNAOfObject(obj))))
          if (indef_cases & (PowersOfTwo_GNA-->(GetGNAOfObject(obj))))
                its_score = its_score + SCORE__GNA;

            match_scores-->i = match_scores-->i + its_score;
            #Ifdef DEBUG;
            if (parser_trace >= 4) print "     ", (The) match_list-->i, " (", match_list-->i,
              ") in ", (the) its_owner, " : ", match_scores-->i, " points^";
            #Endif; ! DEBUG
        }
     }

    for (i=0 : i<number_matched : i++) {
        while (match_list-->i == -1) {
            if (i == number_matched-1) { number_matched--; break; }
            for (j=i : j<number_matched : j++) {
                match_list-->j = match_list-->(j+1);
                match_scores-->j = match_scores-->(j+1);
            }
            number_matched--;
        }
    }
];




! ==============================================================================================================
! PrintCommand prints the name of the player object in the wrong way - Has been changed
! ==============================================================================================================


! ----------------------------------------------------------------------------
!  PrintCommand reconstructs the command as it presently reads, from
!  the pattern which has been built up
!
!  If from is 0, it starts with the verb: then it goes through the pattern.
!  The other parameter is "emptyf" - a flag: if 0, it goes up to pcount:
!  if 1, it goes up to pcount-1.
!
!  Note that verbs and prepositions are printed out of the dictionary:
!  and that since the dictionary may only preserve the first six characters
!  of a word (in a V3 game), we have to hand-code the longer words needed.
!
!  (Recall that pattern entries are 0 for "multiple object", 1 for "special
!  word", 2 to REPARSE_CODE-1 are object numbers and REPARSE_CODE+n means the
!  preposition n)
! ----------------------------------------------------------------------------

[ PrintCommand from i k spacing_flag;
    if (from == 0) {
        i = verb_word;
        if (LanguageVerb(i) == 0)
            if (PrintVerb(i) == 0) print (address) i;
        from++; spacing_flag = true;
    }

    for (k=from : k<pcount : k++) {
        i = pattern-->k;
        if (i == PATTERN_NULL) continue;
        if (spacing_flag) print (char) ' ';
        if (i ==0 ) { print (string) THOSET__TX; jump TokenPrinted; }
        if (i == 1) { print (string) THAT__TX;   jump TokenPrinted; }
        if (i >= REPARSE_CODE)
            print (address) No__Dword(i-REPARSE_CODE);

! ¤#¤ Added one line
      else if(i==player) print (string) MYSELF__TX;

        else
            if (i in compass && LanguageVerbLikesAdverb(verb_word))
                LanguageDirection (i.door_dir); ! the direction name as adverb
            else
                print (the) i;
      .TokenPrinted;
        spacing_flag = true;
    }
];


! ==============================================================================================================
! General bug fix, which became necessary with the Swedish/Norwegian grammar
! ==============================================================================================================

! ----------------------------------------------------------------------------
!  The CantSee routine returns a good error number for the situation where
!  the last word looked at didn't seem to refer to any object in context.
!
!  The idea is that: if the actor is in a location (but not inside something
!  like, for instance, a tank which is in that location) then an attempt to
!  refer to one of the words listed as meaningful-but-irrelevant there
!  will cause "you don't need to refer to that in this game" rather than
!  "no such thing" or "what's 'it'?".
!  (The advantage of not having looked at "irrelevant" local nouns until now
!  is that it stops them from clogging up the ambiguity-resolving process.
!  Thus game objects always triumph over scenery.)
! ----------------------------------------------------------------------------

[ CantSee  i w e;
    saved_oops=oops_from;

    if (scope_token ~= 0) {
        scope_error = scope_token;
        return ASKSCOPE_PE;
    }

    wn--; w = NextWord();
    e = CANTSEE_PE;
! ¤#¤ One line changed
!    if (w==pronoun_word)
    if (w==pronoun_word && ~~ TestScope(pronoun_obj))  ! TestScope condition added
    {
        pronoun__word = pronoun_word; pronoun__obj = pronoun_obj;
        e = ITGONE_PE;
    }
    i = actor; while (parent(i) ~= 0) i = parent(i);

    wn--;
    if (i has visited && Refers(i,wn) == 1) e = SCENERY_PE;
    else {
        Descriptors();  ! skip past THE etc
        if (i has visited && Refers(i,wn) == 1) e = SCENERY_PE;
    }
    wn++;
    if (etype > e) return etype;
    return e;
];


! ==============================================================================================================
! The Main function to figure out the GNA of an object - Has been changed
! ==============================================================================================================

[ GetGNAOfObject obj case gender;
! ¤#¤ Line changed
!    if (obj hasnt animate) case = 6;
    if (obj hasnt animate) case = 8;
    if (obj has male) gender = male;
    if (obj has female) gender = female;
    if (obj has neuter) gender = neuter;
! ¤#¤ New line added
    if (obj has uter) gender = uter;
    if (gender == 0) {
        if (case == 0) gender = LanguageAnimateGender;
        else gender = LanguageInanimateGender;
    }
    if (gender == female)   case = case + 1;
! ¤#¤ Section changed
!    if (gender == neuter) case = case + 2;
!    if (obj has pluralname) case = case + 3;
    if (gender == uter) case = case + 2;
    if (gender == neuter) case = case + 3;
    if (obj has pluralname) case = case + 4;
    return case;
];



! ==============================================================================================================
! Uses LanguagePronouns - Has been changed
! ==============================================================================================================


[ PronounNotice obj x bm;

   if (obj == player) return;

   #ifdef EnglishNaturalLanguage;
   PronounOldEnglish();
   #endif;

! ¤#¤ Changed one line
!   bm = PowersOfTwo_TB-->(GetGNAOfObject(obj));
   bm = PowersOfTwo_GNA-->(GetGNAOfObject(obj));
! ¤#¤ Added one line, to allow for an object to have two genders
   bm = bm | (PowersOfTwo_GNA-->(GetSecondaryGNAOfObject(obj)));

   for (x = 1 : x <= LanguagePronouns-->0: x = x+3)
       if (bm & (LanguagePronouns-->(x+1)) ~= 0)
           LanguagePronouns-->(x+2) = obj;

   #ifdef EnglishNaturalLanguage;
   itobj  = PronounValue('it');  old_itobj  = itobj;
   himobj = PronounValue('him'); old_himobj = himobj;
   herobj = PronounValue('her'); old_herobj = herobj;
   #endif;
];



! ==============================================================================================================
! Uses LanguageGNAsToArticles - Has been modified
! ==============================================================================================================


[ PrefaceByArticle o acode pluralise capitalise  i artform findout;
! ¤#¤ Routine entirely rewritten. Capitalise and findout are now meaningless.
   i=(acode+short_name_case*LanguageCases);
   capitalise = findout;
   if (o.&articles~=0) {
     if(o.#articles>WORDSIZE) {
       if(metaclass(o.&articles-->i)==String) {
         print (string) o.&articles-->i, " ";
         if (pluralise) return;
       }
       jump JustSN;
     }
     else if(metaclass(o.articles)==String) {
       print (string) o.articles, " ";
       if (pluralise) return;
       jump JustSN;
     }
     else if(metaclass(o.articles)==Routine) {
       if(PrintOrRun(o, articles)) {
         print " ";
         if (pluralise) return;
       }
       jump JustSN;
     }
   }
   if(indef_mode==false && o hasnt def_article)
     jump JustSN;
   i = GetGNAOfObject(o);


! ¤#¤ Changes
!   if (pluralise)
!   {   if (i<3 || (i>=6 && i<9)) i = i + 3;
!   }
   if (pluralise)
   {   if (i<4 || (i>=8 && i<12)) i = i + 4;
   }
! ¤#¤ End of change

   i = LanguageGNAsToArticles-->i;

   artform = LanguageArticles
             + 3*WORDSIZE*LanguageContractionForms*(short_name_case + i*LanguageCases);

! ¤#¤ Whole section removed, since it only applies if LanguageContractionForms > 1

   print (string) artform-->acode;
   if (pluralise) return;
.JustSN;
   print (PSN__) o;
];

[ Indefart o i j;
! ¤#¤ Routine entirely rewritten
   i = indef_mode; indef_mode = true;
   j=caps_mode; caps_mode=false;
   if (o has proper) {
     indef_mode = NULL; print (PSN__) o;
   } else {
     PrefaceByArticle(o, 2);
   }
   indef_mode = i;
   caps_mode=j;
];

[ _IndefCall o;
! New routine. Needed because PrintToBuffer can only take one argument for
! a routine in Glulx mode.
  PrefaceByArticle(o, 2);
];

[ CInDefart o i j;
! ¤#¤ Routine entirely rewritten
   i = indef_mode; indef_mode = true;
   j=caps_mode; caps_mode=true;
   if (~~o ofclass Object) {
      indef_mode = NULL;
      print (PSN__) o;
   } else {
      if(o has proper) {
          indef_mode = NULL;
          PrintToBuffer(StorageForShortName, 160, PSN__, o);
      } else
          PrintToBuffer(StorageForShortName, 160, _IndefCall, o);
      if(caps_mode == true)
         StorageForShortName->WORDSIZE =
            UpperCase(StorageForShortName->WORDSIZE);
      for (o=0 : o<StorageForShortName-->0 : o++)
           print (char) StorageForShortName->(o+WORDSIZE);
   }
   indef_mode = i;
   caps_mode=j;
];

[ Defart o   i j;
! ¤#¤ Routine entirely rewritten
  i = indef_mode; indef_mode = false;
  j = caps_mode; caps_mode = false;
  if (o == player) {
    print (string) YOURSELF__TX;
  } else if ((~~o ofclass Object) || o has proper) {
    indef_mode = NULL;
    print (PSN__) o;
  } else
    PrefaceByArticle(o, 1);
  indef_mode = i;
  caps_mode=j;
];
[ CDefart o   i j;
! ¤#¤ Routine entirely rewritten
  i = indef_mode; indef_mode = false;
  j = caps_mode; caps_mode = true;
  if (o == player) {
      PrintToBuffer(StorageForShortName, 160, YOU__TX);
  }
  else if((~~o ofclass Object) || o has proper) {
      indef_mode = NULL;
      PrintToBuffer(StorageForShortName, 160, PSN__, o);
  } else
      PrintToBuffer(StorageForShortName, 160, PrefaceByArticle, o, 0);
  if(caps_mode == true)
     StorageForShortName->WORDSIZE =
        UpperCase(StorageForShortName->WORDSIZE);
  PrintBuffer(StorageForShortName);
  indef_mode = i;
  caps_mode=j;
];



! ####################################################
! Replacements for routines in Verblibm.h
! ####################################################




! These things are fixed:
!   "print WHOM__TX" replaced by "print (whichorwhom) obj"
!   "print WHICH__TX" replaced by "print (whichorwhom) obj"


! ==============================================================================================================
! Uses WHOM_TX and WHICH_TX - Has been modified.
! ==============================================================================================================


[ WriteAfterEntry o depth stack_p
    p recurse_flag parenth_flag eldest_child child_count combo;

    inventory_stage = 2;
    if (c_style & PARTINV_BIT) {
        if (o.invent && RunRoutines(o, invent))                 ! These lines
            if (c_style & NEWLINE_BIT) ""; else rtrue;          ! added

        combo = 0;
        if (o has light && location hasnt light) combo=combo+1;
        if (o has container && o hasnt open)     combo=combo+2;
        if ((o has container && (o has open || o has transparent))
            && (child(o)==0))                    combo=combo+4;
        if (combo) L__M(##ListMiscellany, combo, o);
    }   ! end of PARTINV_BIT processing

    if (c_style & FULLINV_BIT) {
        if (o.invent && RunRoutines(o, invent))
            if (c_style & NEWLINE_BIT) ""; else rtrue;

        if (o has light && o has worn) { L__M(##ListMiscellany, 8);     parenth_flag = true; }
        else {
            if (o has light)           { L__M(##ListMiscellany, 9, o);  parenth_flag = true; }
            if (o has worn)            { L__M(##ListMiscellany, 10, o); parenth_flag = true; }
        }

        if (o has container)
            if (o has openable) {
                if (parenth_flag) print (string) AND__TX;
                else              L__M(##ListMiscellany, 11, o);
                if (o has open)
                    if (child(o)) L__M(##ListMiscellany, 12, o);
                    else          L__M(##ListMiscellany, 13, o);
                else
                    if (o has lockable && o has locked) L__M(##ListMiscellany, 15, o);
                    else                                L__M(##ListMiscellany, 14, o);
                parenth_flag = true;
            }
            else
                if (child(o)==0 && o has transparent)
                    if (parenth_flag) L__M(##ListMiscellany, 16, o);
                    else              L__M(##ListMiscellany, 17, o);

        if (parenth_flag) print ")";
    }   ! end of FULLINV_BIT processing

    if (c_style & CONCEAL_BIT) {
        child_count = 0;
        objectloop (p in o)
            if (p hasnt concealed && p hasnt scenery) { child_count++; eldest_child = p; }
    }
    else { child_count = children(o); eldest_child = child(o); }

    if (child_count && (c_style & ALWAYS_BIT)) {
        if (c_style & ENGLISH_BIT) L__M(##ListMiscellany, 18, o);
        recurse_flag = true;
    }

    if (child_count && (c_style & RECURSE_BIT)) {
        if (o has supporter) {
            if (c_style & ENGLISH_BIT) {
                if (c_style & TERSE_BIT) L__M(##ListMiscellany, 19, o);
                else                     L__M(##ListMiscellany, 20, o);
! ¤#¤ Changed two lines, one of which was removed completely
!                if (o has animate) print (string) WHOM__TX;
!                else print (string) WHICH__TX;
                print (whichorwhom) o, " ";
            }
            recurse_flag = true;
        }
        if (o has container && (o has open || o has transparent)) {
            if (c_style & ENGLISH_BIT) {
                if (c_style & TERSE_BIT) L__M(##ListMiscellany, 21, o);
                else                     L__M(##ListMiscellany, 22, o);
! ¤#¤ Changed two lines, one of which was removed completely
!                if (o has animate) print (string) WHOM__TX;
!                else print (string) WHICH__TX;
                print (whichorwhom) o, " ";
            }
            recurse_flag = true;
        }
    }

    if (recurse_flag && (c_style & ENGLISH_BIT))
        if (child_count > 1 || eldest_child has pluralname) print (string) ARE2__TX;
        else                                                print (string) IS2__TX;

    if (c_style & NEWLINE_BIT) new_line;

    if (recurse_flag) {
        o = child(o);
        #Ifdef TARGET_ZCODE;
        @push lt_value; @push listing_together; @push listing_size;
        #Ifnot; ! TARGET_GLULX;
        @copy lt_value sp; @copy listing_together sp; @copy listing_size sp;
        #Endif;
        lt_value = 0;   listing_together = 0;   listing_size = 0;
        WriteListR(o, depth+1, stack_p);
        #Ifdef TARGET_ZCODE;
        @pull listing_size; @pull listing_together; @pull lt_value;
        #Ifnot; ! TARGET_GLULX;
        @copy sp listing_size; @copy sp listing_together; @copy sp lt_value;
        #Endif;
        if (c_style & TERSE_BIT) print ")";
    }
];


! ==============================================================================================================
! Prints library messages with too few params - Has been modified.
! ==============================================================================================================


[ Objects1Sub i j f;
    L__M(##Objects, 1);
    objectloop (i has moved) {
       f = 1; print (the) i; j = parent(i);
        if (j) {
           if (j == player) {
! ¤#¤ Changed two lines
!                if (i has worn) L__M(##Objects, 3);
!                else L__M(##Objects, 4);
                if (i has worn) L__M(##Objects, 3, i);
                else L__M(##Objects, 4, i);
                jump Obj__Ptd;
            }
! ¤#¤ Changed one line
!            if (j has animate)   { L__M(##Objects, 5); jump obj__ptd; }
            if (j has animate)   { L__M(##Objects, 5, j); jump obj__ptd; }
            if (j has visited)   { L__M(##Objects, 6, j); jump Obj__Ptd; }
            if (j has container) { L__M(##Objects, 8, j); jump Obj__Ptd; }
            if (j has supporter) { L__M(##Objects, 9, j); jump Obj__Ptd; }
            if (j has enterable) { L__M(##Objects, 7, j); jump Obj__Ptd; }
        }
        L__M(##Objects, 10);

      .Obj__Ptd;

        new_line;
    }
    if (f == 0) L__M(##Objects, 2);
];
