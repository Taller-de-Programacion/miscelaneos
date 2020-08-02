_some_app() {
	local cur prev words cword
	_init_completion || return
    COMPREPLY=($(compgen -W "-v -i -o --input --output" -- "$cur"))
}

complete -F _some_app ./some-app
