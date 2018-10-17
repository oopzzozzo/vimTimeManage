recursive display
detail filename in dir
-mv debug
-rm
zip log


vimtime <dir/fn>

    2 files.
    0.03s spent.

filename                open     edit           normal          cmd    
---------------------------------------------------------------------
fn1                     02°45'   40'05/01°00'   13'10"/20'00"   10'30"
fn2                     02°45'   40'05/01°00'   13'10"/20'00"   10'30"
---------------------------------------------------------------------
SUM                     02°45'   01°20/02°00'   26'20"/40'00"   21'00"


vimtime -[dmy] dir/fn

    2 files.
    0.03s spent.

file1name              open     edit           normal          cmd   
Fri Oct 23 2009        02°45'   40'05/01°00'   13'10"/20'00"   10'30"
Fri Oct 28 2009        02°45'   40'05/01°00'   13'10"/20'00"   10'30" 

file2name              open     edit           normal          cmd   
Fri Oct 23 2009        02°45'   40'05/01°00'   13'10"/20'00"   10'30"
Fri Oct 28 2009        02°45'   40'05/01°00'   13'10"/20'00"   10'30" 

vimtime -mv[uf] <fn1> <fn2>

fn1 (un)aliased(renamed) as fn2.
    0.03s spent.

vimtime -rm[f] <fn>

fn hidden(revealed/erased).
    0.02s spent.

.vimtime

name1 open timestamp
name1 edit timestamp
name1 norm timestamp
name1 scmd timestamp
name1 ecmd timestamp
name1 spar timestamp
name1 back timestamp
name1 clos timestamp

