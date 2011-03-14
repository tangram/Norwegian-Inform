! ----------------------------------------------------------------------------
!
!  GRAMMAR:  Grammar table entries for the standard verbs library.
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
!   In your game file, Include these library files in this order:
!       Include "NorAlpha"; ! Must be included at the *very* beginning of game
!                           ! even before Story and Headline are defined
!       Include "Parser";
!       Include "VerbLib";
!       Include "NorRout";
!       Include "NorGram";
!
! ==============================================================================

System_file;

#ifndef LIBRARY_NORWEGIAN;
  #ifdef LIBRARY_ENGLISH;
    Message "ERROR: You must use 'inform +language_name=Norwegian ...' to compile.";
  #ifnot;
    Message "ERROR: Files MUST be included in this order: Parser, Verblib, NorRout, NorGram.";
    abort;
  #endif;
#endif;

! ----------------------------------------------------------------------------
!  The "meta-verbs", commands to the game rather than in the game, come first:
! ----------------------------------------------------------------------------

Verb meta 'a//' 'avslutt' 'slutt' 'dø'
                *                                -> Quit
                * 'spill'/'spillet'              -> Quit;
Verb meta 'omstart'
                *                                -> Restart;
Verb meta 'full' 'fulle'
                *                                -> FullScore
                * 'poeng'                        -> FullScore;
!Verb meta 'hjelp'
!                *                                -> Help;
Verb meta 'last' 'laste'
                * 'lagret' 'spill'               -> Restore
                * 'spill'                        -> Restore
                *                                -> Restore;
Verb meta 'innspilling'
                *                                -> CommandsOn
                * 'på'                           -> CommandsOn
                * 'av'                           -> CommandsOff;
Verb meta 'korte'
                *                                -> LMode3
                * 'beskrivelser'                 -> LMode3;
Verb meta 'lange'
                *                                -> LMode2
                * 'beskrivelser'                 -> LMode2;
Verb meta 'beskjeder'
                * 'på'                           -> NotifyOn
                * 'av'                           -> NotifyOff;
Verb meta 'normale'
                *                                -> LMode1
                * 'beskrivelser'                 -> LMode1;
Verb meta 'poeng'
                *                                -> Score;
Verb meta 'pronomen'
                *                                -> Pronouns;
Verb meta 'lagre'
                * 'spill'                        -> Save
                * 'spillet'                      -> Save
                *                                -> Save;
Verb meta 'transkript'
                *                                -> TranscribeOn
                * 'på'                           -> TranscribeOn
                * 'vis'                          -> TranscribeShow
                * 'av'                           -> TranscribeOff;
Verb meta 'avspilling'
                *                                -> CommandsRead;
Verb meta 'utskrift'
                *                                -> ScriptOn
                * 'av'                           -> ScriptOff
                * 'på'                           -> ScriptOn;
Verb meta 'verifiser'
                *                                -> Verify;
Verb meta 'versjon'
                *                                -> Version;




#IFNDEF NO_PLACES;
Verb meta 'objekter'
                *                                -> Objects;
Verb meta 'steder'
                *                                -> Places;
#ENDIF;

! ----------------------------------------------------------------------------
!  Debugging grammar
! ----------------------------------------------------------------------------

#Ifdef DEBUG;
Verb meta 'actions'
    *                                           -> ActionsOn
    * 'on'                                      -> ActionsOn
    * 'off'                                     -> ActionsOff;
Verb meta 'changes'
    *                                           -> ChangesOn
    * 'on'                                      -> ChangesOn
    * 'off'                                     -> ChangesOff;
Verb meta 'gonear'
    * noun                                      -> Gonear;
Verb meta 'goto'
    * number                                    -> Goto;
Verb meta 'random'
    *                                           -> Predictable;
Verb meta 'routines' 'messages'
    *                                           -> RoutinesOn
    * 'on'                                      -> RoutinesOn
    * 'off'                                     -> RoutinesOff;
Verb meta 'scope'
    *                                           -> Scope
    * noun                                      -> Scope;
Verb meta 'showobj'
    *                                           -> Showobj
    * number                                    -> Showobj
    * multi                                     -> Showobj;
Verb meta 'showverb'
    * special                                   -> Showverb;
Verb meta 'timers' 'daemons'
    *                                           -> TimersOn
    * 'on'                                      -> TimersOn
    * 'off'                                     -> TimersOff;
Verb meta 'trace'
    *                                           -> TraceOn
    * number                                    -> TraceLevel
    * 'on'                                      -> TraceOn
    * 'off'                                     -> TraceOff;
Verb meta 'abstract'
    * noun 'to' noun                            -> XAbstract;
Verb meta 'purloin'
    * multi                                     -> XPurloin;
Verb meta 'tree'
    *                                           -> XTree
    * noun                                      -> XTree;
Verb meta 'falkeblikk'
    *                                           -> CheckGame
    * noun                                      -> CheckGame
    * number                                    -> CheckGame
    * 'hvilke'                                  -> CheckGameSelect;


#Ifdef TARGET_GLULX;
Verb meta 'glklist'
    *                                           -> Glklist;
#Endif; ! TARGET_

#Endif; ! DEBUG

! ----------------------------------------------------------------------------
!  And now the game verbs.
! ----------------------------------------------------------------------------

[ Exitable ;
  if(player in noun) rtrue;
  rfalse;
];

[ ADirection; if (noun in compass) rtrue; rfalse; ];

! TO ADD: 'før' 'løft' 'heis' 'reis' 'støtt'

Verb 'angrip' 'knus' 'mos' 'knekk' 'brekk' 'drep' 'ødelegg'
     'bryt' 'myrd' 'myrde' 'kvest' 'kverk' 'smadre' 'spark'
                * noun                                      -> Attack
                * noun 'med' held                           -> Attack;
Verb 'slåss'
                * 'med' noun                                -> Attack;
Verb 'be'
                *                                           -> Pray
                * 'om' 'å' 'få' noun 'av' creature          -> AskFor reverse
                * 'om' noun 'av' creature                   -> AskFor reverse
                * creature 'om' noun                        -> AskFor
                * creature 'om' 'å' 'gi' 'meg' noun         -> AskFor
                * creature 'gi' 'meg' noun                  -> AskFor;
Verb 'befal' 'anmod' 'beordre'
                * creature 'om' noun                        -> AskFor
                * creature 'om' 'å' 'gi' 'meg' noun         -> AskFor
                * creature 'gi' 'meg' noun                  -> AskFor;
Verb 'fortell' 'forklar' 'bekjenn' 'berett' 'tal'
                * 'for' creature 'om' topic                 -> Tell
                * 'om' topic 'for' creature                 -> Tell reverse;
Verb 'nevn'
                * topic 'for' creature                      -> Tell reverse;
Verb 'prat' 'snakk'
                * 'om' topic 'for'/'til' creature           -> Tell reverse
                * 'til' creature 'om' topic                 -> Tell;
Verb 'beskjær'
                * noun                                      -> Cut;
Verb 'bind' 'fest' 'knyt' 'knytt'
                * noun                                      -> Tie
                * 'fast' noun 'i//' noun                    -> Tie
                * noun 'i//' noun                           -> Tie;
Verb 'blås'
                * 'på'/'i//' held                           -> Blow;
Verb 'brenn' 'tenn'
                * noun                                      -> Burn
                * 'på' noun                                 -> Burn
                * 'på' noun 'med' held                      -> Burn
                * noun 'med' held                           -> Burn;
Verb 'kle'
                * 'på' clothing                             -> Wear;
Verb 'dra' 'trekk'
                * 'i//' noun                                -> Pull
                * noun                                      -> Pull;
Verb 'drikk'
                * noun                                      -> Drink
                * 'av' noun                                 -> Drink;
Verb 'dump'
                * multiheld                                 -> Drop
                * multiexcept 'i//'/'inni' noun             -> Insert
                * multiexcept 'inn' 'i//' noun              -> Insert
                * multiexcept 'på'/'oppå' noun              -> PutOn
                * multiexcept 'opp'/'oppe' 'på' noun        -> PutOn;
Verb 'flytt'
                * noun                                      -> Push
                * 'på' noun                                 -> Push
                * noun noun                                 -> PushDir
                * noun 'til' noun                           -> Transfer;
Verb 'spør'
                * creature 'om' topic                       -> Ask
                * 'om' 'å' 'få' noun 'av' creature          -> AskFor reverse;
Verb 'fyll'
                * noun                                      -> Fill
                * 'på'/'opp' noun                           -> Fill;
Verb 'gi' 'tilby' 'by' 'betal'
                * held 'til' creature                       -> Give
                * 'bort' held 'til' creature                -> Give
                * creature held                             -> Give reverse;
Verb 'gjennomsøk'
                * noun                                      -> Search;
Verb 'gni' 'gnikk' 'poler' 'polere'
     'tørk' 'stryk' 'skrubb' 'puss'
                * noun                                      -> Rub
                * 'av'/'på' noun                            -> Rub;
Verb 'grav'
                * noun                                      -> Dig
                * 'i//'/'opp' noun 'med' held               -> Dig;
Verb 'gyng' 'gynge' 'sving'
                * noun                                      -> Swing
                * 'på' noun                                 -> Swing;
Verb 'gå' 'løp' 'stikk' 'sprint'
                *                                           -> VagueGo
                * 'inn'                                     -> GoIn
                * 'ut'                                      -> Exit
                * noun=ADirection                           -> Go
                * 'til'/'mot' noun=ADirection               -> Go
                * 'opp' 'fra' noun                          -> Exit
                * noun                                      -> Enter
                * 'på' noun                                 -> Enter
                * 'opp' 'på' noun                           -> Enter
                * 'inn' 'i//'/'gjennom'/'igjennom' noun     -> Enter
                * 'gjennom'/'igjennom' noun                 -> Enter
                * 'av' noun                                 -> Exit
                * 'ut' 'av' noun                            -> Exit
                * 'ut' 'i//'/'gjennom'/'igjennom' noun      -> Exit
                * 'ned' 'fra' noun                          -> Exit;
Verb 'stig' 'klyv'
                * 'på' noun                                 -> Enter
                * 'opp'                                     -> Exit
                * 'opp' 'fra' noun                          -> Exit
                * 'inn'                                     -> GoIn
                * 'ut'                                      -> Exit
                * 'opp' 'på' noun                           -> Enter
                * 'inn' 'i//'/'gjennom'/'igjennom' noun     -> Enter
                * 'av' noun                                 -> Exit
                * 'ut' 'ur' noun                            -> Exit
                * 'ut' 'i//'/'gjennom'/'igjennom' noun      -> Exit
                * 'ned' 'fra' noun                          -> Exit;
Verb 'hopp' 'spring'
                *                                           -> Jump
                * 'over' noun                               -> JumpOver;
Verb 'inn'
                *                                           -> GoIn;
Verb 'ja' 'j//' 'ok' 'greit' 'mm' 'jepp' 'jo' 'okei' 'ålreit' 'javel'
                *                                           -> Yes;
Verb 'kast' 'sleng'
                * multiheld                                 -> Drop
                * multiexcept 'i//'/'inni' noun             -> Insert
                * multiexcept 'inn' 'i//' noun              -> Insert
                * 'inn'/'ned' multiexcept 'i//'/'inni' noun
                                                            -> Insert
                * 'inn'/'ned' multiexcept 'inne' 'i' noun
                                                            -> Insert
                * 'opp'/'ned' multiexcept 'på'/'oppå' noun
                                                            -> PutOn
                * 'opp'/'ned' multiexcept 'opp'/'oppe' 'på' noun
                                                            -> PutOn
                * held 'på'/'mot' noun                      -> ThrowAt
                * held 'til' creature                       -> ThrowAt;
Verb 'klem'
                * creature                                  -> Kiss
                * 'på' creature                             -> Kiss
                * noun                                      -> Squeeze
                * 'på' noun                                 -> Squeeze;
Verb 'klatre'
                * 'på'/'over'/'oppover'/'i' noun            -> Climb
                * 'ut'/'av'/'ned'                           -> Exit
                * 'av'/'ned' 'fra' noun                     -> Exit
                * 'opp' 'på' noun                           -> Climb
                * 'inn' 'i//'/'gjennom'/'igjennom' noun     -> Enter
                * 'gjennom'/'igjennom' noun                 -> Enter;
Verb 'kryss'
                * noun                                      -> Enter;
Verb 'omfavn' 'omfavne'
                * creature                                  -> Kiss
                * noun                                      -> Squeeze;
Verb 'kyss' 'suss'
                * creature                                  -> Kiss;
Verb 'kjøp' 'handle'
                * 'inn' noun                                -> Buy
                * noun                                      -> Buy;
Verb 'lei' 'unnskyld' 'beklager' 'sorry'
                * 'meg'                                     -> Sorry
                *                                           -> Sorry;
Verb 'list'
                * 'eiendeler'/'saker'/'ting'                -> Inv
                * 'mine' 'eiendeler'/'saker'/'ting'         -> Inv
                *                                           -> Inv
                * 'lang'                                    -> InvTall
                * 'bred'                                    -> InvWide;
Verb 'eiendeler' 'e//'
                *                                           -> Inv
                * 'lang'                                    -> InvTall
                * 'bred'                                    -> InvWide;
Verb 'lukk'
                * noun                                      -> Close
                * 'igjen' noun                              -> Close;
Verb 'lukt' 'sniff'
                *                                           -> Smell
                * 'på'/'i//' noun                           -> Smell;
Verb 'lytt' 'hør'
                *                                           -> Listen
                * 'på' noun                                 -> Listen;
Verb 'legg'
                * 'meg'/'deg' 'på'/'i//' noun               -> Enter
                * 'meg'/'deg' 'oppe' 'på'/'i//' noun        -> Enter
                * multiexcept 'inni'/'i//' noun             -> Insert
                * multiexcept 'inne' 'i//' noun             -> Insert
                * multiexcept 'på'/'oppå' noun              -> PutOn
                * multiexcept 'oppe' 'på' noun              -> PutOn
                * 'ned'/'bort' multiheld                    -> Drop
                * 'ned'/'inn' multiheld 'i//'/'inni' noun
                                                            -> Insert
                * 'ned'/'inn' multiheld 'inne' 'i//' noun
                                                            -> Insert
                * 'ned'/'opp' multiexcept 'på'/'oppå' noun
                                                            -> PutOn
                * 'ned'/'opp' multiexcept 'oppe' 'på' noun
                                                            -> PutOn
                * multiheld                                 -> Drop
                * 'fra' 'deg'/'meg' multiheld               -> Drop;

Verb 'forlat'
                * noun=Exitable                             -> Exit
                * multiheld                                 -> Drop
                * multiexcept 'i//'/'inni' noun             -> Insert
                * multiexcept 'inne' 'i//' noun             -> Insert
                * multiexcept 'på'/'oppå' noun              -> PutOn
                * multiexcept 'oppe' 'på' noun              -> PutOn;
Verb 'les'
                * noun                                      -> Examine
                * 'i//'/'på' noun                           -> Examine
                * 'om' topic 'i//' noun                     -> Consult;
Verb 'lås'
                * noun 'med' held                           -> Lock
                * 'opp' noun 'med' held                     -> Unlock;
Verb 'fillern' 'søren' 'fankern' 'rakkern'
     'fyttirakkern' 'dæven' 'dæsken' 'fytti'
     'fyttigrisen' 'døtte' 'fy' 'pokker'
                *                                           -> Mild
                * topic                                     -> Mild;
Verb 'nei' 'neivel'
                *                                           -> No;
Verb 'plasser' 'putt'
                * multiexcept 'i//'/'inni' noun             -> Insert
                * multiexcept 'inne' 'i//' noun             -> Insert
                * multiexcept 'på'/'oppå' noun              -> PutOn
                * multiexcept 'oppe' 'på' noun              -> PutOn;
Verb 'reis'
                * 'på' 'meg'/'deg'                          -> Exit
                * 'meg'/'deg'                               -> Exit
                * 'meg'/'deg' 'opp'                         -> Exit
                * 'meg'/'deg' 'fra' noun                    -> Exit;
Verb 'rør' 'berør'
                * noun                                      -> Touch
                * 'på'/'ved' noun                           -> Touch;
Verb 'svøm' 'dykk'
                *                                           -> Swim;
Verb 'sitt' 'ligg'
                * 'på' noun                                 -> Enter
                * 'oppe'/'opp' 'på' noun                    -> Enter
                * 'inne' 'i//' noun                         -> Enter
                * 'i//'/'inni'/'på' noun                    -> Enter;
Verb 'syng' 'trall' 'nynn' 'jodle'
                *                                           -> Sing;
Verb 'faen' 'satan' 'helvete' 'hælvete' 'jævla' 'jævel'
     'fitte' 'kuk' 'kukk' 'ræv' 'pul' 'forpulte' 'knull'
                *                                           -> Strong
                * topic                                     -> Strong;
Verb 'skjær' 'kutt'
                * noun                                      -> Cut
                * 'av'/'opp'/'i//' noun                     -> Cut;
Verb 'slå'
                * 'opp' topic 'i//' noun                    -> Consult
                * noun                                      -> Attack
                * noun 'med' held                           -> Attack
                * 'istykker' noun                           -> Attack
                * 'i//' 'stykker' noun                      -> Attack
                * 'til'/'på' switchable                     -> SwitchOn
                * 'av'/'fra' switchable                     -> SwitchOff
                * 'til'/'på' noun                           -> Attack;
Verb 'slipp'
                * multiheld                                 -> Drop
                * 'ned' multiexcept 'på'/'oppå' noun        -> PutOn
                * 'ned' multiexcept 'oppe' 'på' noun        -> PutOn
                * 'ned'/'inn' multiexcept 'i//'/'inni' noun
                                                            -> Insert
                * 'ned'/'inn' multiexcept 'inne' 'i//' noun
                                                            -> Insert
                * multiexcept 'på'/'oppå' noun              -> PutOn
                * multiexcept 'oppe' 'på' noun              -> PutOn
                * multiexcept 'i//'/'inni' noun             -> Insert
                * multiexcept 'inne' 'i//' noun             -> Insert;
Verb 'smak' 'sutt' 'slikk' 'sug'
                * noun                                      -> Taste
                * 'på' noun                                 -> Taste;
Verb 'kjenn' 'hold' 'føl'
                * noun                                      -> Touch
                * 'på' noun                                 -> Touch;
Verb 'beføl'
                * noun                                      -> Touch;
Verb 'snurr' 'vri' 'drei' 'skru'
                * noun                                      -> Turn
                * 'på'/'av' noun                            -> Turn
                * 'på' switchable                           -> SwitchOn
                * 'av' switchable                           -> SwitchOff;
Verb 'sov' 'hvil'
                *                                           -> Sleep;
Verb "start"
                * noun                                      -> Switchon
                * 'på' 'nytt'                               -> Restart;
Verb 'stå'
                *                                           -> Exit
                * 'opp'                                     -> Exit
                * 'på' noun                                 -> Enter
                * 'i' noun                                  -> Enter;
Verb 'still'
                * 'meg'/'deg' 'på'/'i//' noun               -> Enter
                * 'meg'/'deg' 'opp'                         -> Exit
                * multiexcept 'på'/'oppå' noun              -> PutOn
                * multiexcept 'oppe' 'på' noun              -> PutOn
                * 'ned' multiheld                     -> Drop
                * 'ned'/'opp' multiexcept 'på'/'oppå' noun
                                                            -> PutOn
                * 'ned'/'opp' multiexcept 'oppe' 'på' noun
                                                            -> PutOn
                * 'ned'/'in' multiexcept 'i//'/'inni' noun
                                                            -> Insert
                * 'ned'/'in' multiexcept 'inne' 'i//' noun
                                                            -> Insert
                * multiexcept 'i//'/'inni' noun             -> Insert
                * multiexcept 'inne' 'i//' noun             -> Insert
                * noun                                      -> Set
                * 'in' noun                                 -> Set
                * 'in' noun 'på' special                    -> SetTo;
Verb 'steng'
                * noun                                      -> Close
                * 'igjen' noun                              -> Close
                * 'av' noun                                 -> SwitchOff;
Verb 'stopp'
                * noun                                      -> SwitchOff;
Verb 'svar'
                * creature topic                            -> Answer;
Verb 'svelg'
                * noun                                      -> Drink;
Verb 'si' 'skrik' 'rop'
                * topic 'til' creature                      -> Answer;
Verb 'sett'
                * 'meg'/'deg' 'på'/'i//' noun               -> Enter
                * 'meg'/'deg' 'oppe' 'på'/'i//' noun        -> Enter
                * noun 'fast'                               -> Tie
                * noun 'fast' 'i//' noun                    -> Tie
                * 'fast' noun 'i//' noun                    -> Tie
                * multiexcept 'på'/'oppå' noun              -> PutOn
                * multiexcept 'oppe' 'på' noun              -> PutOn
                * 'ned' multiheld                           -> Drop
                * 'ned'/'opp' multiexcept 'på'/'oppå' noun
                                                            -> PutOn
                * 'ned'/'opp' multiexcept 'oppe' 'på' noun
                                                            -> PutOn
                * 'ned'/'in' multiexcept 'i//'/'inni' noun
                                                            -> Insert
                * 'ned'/'in' multiexcept 'inne' 'i//' noun
                                                            -> Insert
                * multiexcept 'i//'/'inni' noun             -> Insert
                * multiexcept 'inne' 'i//' noun             -> Insert
                * 'på' switchable                           -> SwitchOn
                * 'på' clothing                             -> Wear
                * noun                                      -> Set
                * noun 'til'/'på' special                   -> SetTo;
Verb 'søk' 'let'
                * 'igjennom'/'i//' noun                     -> Search
                * 'gjennom' noun                            -> Search;
Verb 'ta' 't//' 'plukk'
                * multi                                     -> Take
                * 'tak' 'i//' multi                         -> Take
                * 'opp'/'fram' multi                        -> Take
                * 'av' 'meg'/'deg' noun                     -> Disrobe
                * 'av' worn                                 -> Disrobe
                * worn 'av'                                 -> Disrobe
                * worn 'av' 'meg'/'deg'                     -> Disrobe
                * multiinside 'fra'/'ut'/'i//' noun         -> Remove
                * 'opp'/'fram'/'ut'/'bort' multiinside 'i//'/'fra'/'utfra' noun
                                                            -> Remove
                * 'på' 'meg'/'deg' noun                     -> Wear
                * 'på' clothing                             -> Wear
                * clothing 'på'                             -> Wear
                * clothing 'på' 'meg'/'deg'                 -> Wear
                * 'på' noun                                 -> Touch;
Verb 'stjel'
                * multi                                     -> Take;
Verb 'se' 'kikk' 'k//'
                *                                           -> Look
                * 'deg'/'meg' 'rundt'/'omkring'             -> Look
                * noun=ADirection                           -> Examine
                * 'mot' noun=ADirection                     -> Examine
                * 'på' noun                                 -> Examine
                * 'nærmere' 'på' noun                       -> Examine
                * 'i//'/'inni'/'gjennom'/'igjennom' noun    -> Search
                * 'inn'/'inne' 'i//' noun                   -> Search
                * 'under' noun                              -> LookUnder;
Verb 'trykk' 'knuff' 'press' 'dytt'
                * noun                                      -> Push
                * 'på'/'til' noun                           -> Push
                * noun noun                                 -> PushDir
                * noun 'til' noun                           -> Transfer;
Verb 'tenk' 'gruble'
                *                                           -> Think;
Verb 'tøm' 'tapp'
                * noun                                      -> Empty
                * 'ut'/'utav' noun                          -> Empty
                * 'ut'/'utav' noun 'i//'/'inni'/'på'/'oppå' noun
                                                            -> EmptyT;
Verb 'undersøk' 'u//' 'gransk' 'studer'
                * noun                                      -> Examine;
Verb 'ut'
                *                                           -> Exit
                * 'av' noun                                 -> Exit;
Verb 'våkne'
                *                                           -> Wake
                * 'opp'/'til'                               -> Wake;
Verb 'vift' 'vink' 'veiv'
                *                                           -> WaveHands
                * 'med' held                                -> Wave;
Verb 'vis' 'presenter'
                * creature held                             -> Show reverse
                * 'opp' held 'for'/'til' creature           -> Show
                * held 'for'/'til' creature                 -> Show
                * 'eiendeler'/'saker'/'ting'                -> Inv
                * 'mine' 'eiendeler'/'saker'/'ting'         -> Inv;
Verb 'vekk'
                * creature                                  -> WakeOther
                * 'opp' creature                            -> WakeOther;
Verb 'snu' 'roter'
                * noun                                      -> Turn
                * 'rundt'/'på' noun                         -> Turn;
Verb 'vent' 'z//'
                *                                           -> Wait;
Verb 'spis' 'et' 'sluk'
                * 'opp' held                                -> Eat
                * held                                      -> Eat;
Verb 'åpne'
                * noun                                      -> Open
                * noun 'med' held                           -> Unlock;

! ----------------------------------------------------------------------------
!  Final task: provide trivial routines if the user hasn't already:
! ----------------------------------------------------------------------------

#Stub TimePasses      0;
#Stub Amusing         0;
#Stub DeathMessage    0;
#Stub DarkToDark      0;
#Stub NewRoom         0;
#Stub LookRoutine     0;
#Stub AfterLife       0;
#Stub GamePreRoutine  0;
#Stub GamePostRoutine 0;
#Stub AfterPrompt     0;
#Stub BeforeParsing   0;
#Stub PrintTaskName   1;
#Stub InScope         1;
#Stub UnknownVerb     1;
#Stub PrintVerb       1;
#Stub ParserError     1;
#Stub ParseNumber     2;
#Stub ChooseObjects   2;

#ifdef TARGET_GLULX;
  #Stub IdentifyGlkObject 4;
  #Stub HandleGlkEvent    2;
  #Stub InitGlkWindow     1;
#endif; ! TARGET_GLULX

#IFNDEF PrintRank;
  Constant Make__PR;
#ENDIF;
#IFDEF Make__PR;
  [ PrintRank; "."; ];
#ENDIF;
#IFNDEF ParseNoun;
  Constant Make__PN;
#ENDIF;
#IFDEF Make__PN;
  [ ParseNoun obj; obj=obj; return -1; ];
#ENDIF;
#Default Story    0;
#Default Headline 0;
#IFDEF INFIX;
  #Include "infix";
#ENDIF;

! ==============================================================================

Constant LIBRARY_GRAMMAR;       ! for dependency checking

! ==============================================================================