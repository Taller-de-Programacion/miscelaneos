_some_app() {
    local cur prev words cword
    _init_completion || return

    local VERBOSE_FLAG="-v "
    for word in "${words[@]}"
    do
        if [ "$word" = "-v" ] ; then
            VERBOSE_FLAG=""
        fi
    done

    case $prev in
        -i|--input)
            COMPREPLY=($(compgen -f -X "!*.sh" -- "$cur"))
            return
            ;;
        -o|--output)
            COMPREPLY=($(compgen -d -- "$cur"))
            return
            ;;
        -v)
            COMPREPLY=($(compgen -W "-i -o --input --output" -- "$cur"))
            return
            ;;
    esac
    COMPREPLY=($(compgen -W "$VERBOSE_FLAG -i -o --input --output" -- "$cur"))
}

complete -F _some_app ./some-app
