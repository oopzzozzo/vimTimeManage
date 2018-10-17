function ByShell(cmd)
    write
	call s:vimtime('scmd')
    exec '!' . a:cmd
	call s:vimtime('ecmd')
endfunction
let s:spared=0

function s:vimtime(event) abort
	if expand('%:t') != '.vimtime' && @% != ''
		if a:event != 'back' && s:spared == 1
			call s:vimtime('back')
		endif
		silent exec '!echo %:t '.a:event.' '.localtime().' >> %:p:h/.vimtime'
		if a:event == 'back'
			let s:spared=0
		endif
	endif
endfunction

augroup vimtime
	autocmd BufEnter * call s:vimtime('open')
	autocmd BufLeave,VimLeave * call s:vimtime('clos')
	autocmd InsertEnter * call s:vimtime('edit')
	autocmd InsertLeave * call s:vimtime('norm')
	autocmd CursorHold,CursorHoldI * call s:vimtime('spar') | let s:spared=1 
	autocmd CursorMoved,CursorMovedI * if s:spared==1 | call s:vimtime('back')
augroup END
