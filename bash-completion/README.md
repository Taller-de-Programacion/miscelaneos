# Bash completion básico

Se recomienda leer el tutorial de empaquetado .deb para mejor control de los archivos que se van a instalar.

Muchas aplicaciones de consola utilizan argumentos para interactuar con las mismas, motivo por el cuál se utilizan herramientas de autocompletado. Una de ellas es `complete`. Un ejemplo es *docker*, cuyo autocompletado realiza consultas como el estado de los containers, nombres de las redes virtuales, etc.

Para probar esta aplicación creamos una aplicación `some-app`. Esta aplicación de juguete parsea 3 tipos de argumentos: `-i` o `--input` para entradas, `-o` o `--output` para salidas, y `-v` para activar un flag "verboso".

Compilamos utilizando el makefile. Para autocompletar vamos a usar el comando `complete`


## Autocompletado básico con lista de palabras

Vamos a listar los posibles argumentos: 

~~~{.bash}
complete -W "-v -i -o --input --output" some-app
~~~

* `-W` es para indicar que es una lista de palabras para autocompletar.

* `"-v -i -o --input --output"` es el string con las palabras del autocompletado.

* `some-app` es el nombre de la aplicación

Si ejecutamos

~~~{.bash}
./some-app <tab><tab>
~~~

Vemos como nos sugiere los 5 modificadores que sugerimos. 


## Autocompletar con nombres de archivos

Utilizando el flag `-f` sugerimos nombres de archivos, mientras que con `-d` sugerimos nombres de directorios.

Podemos probar ejecutando

~~~{.bash}
complete -f ./some-app
./some-app <tab><tab>
~~~

Vemos como ahora nos sugiere nombres de archivos para autocompletar

Mientras que si ejecutamos

~~~{.bash}
complete -d ./some-app
./some-app <tab><tab>
~~~

### Filtrar archivos según su extensión

Podemos usar `-X` para excluir términos del autocompletado usando una *regex*.

Por ejemplo, si usamos `complete -f -X '*.sh'` estamos tomando todos los archivos y excluyendo los terminados en `.sh`. Por el contrario `complete -f -X '!*.sh'` nos va a devolver sólo los terminados en `.sh`. Notar el uso comillas simples.


## Autocompletar usando funciones

Las opciones anteriores son prácticas pero no tan dinámicas. Se puede lograr una mucho mayor flexibilidad utilizando funciones para poblar el autocompletado.

Para esto vamos a necesitar un poco de bash y el comando compgen.

### Generar una lista de palabras programáticamente

Necesitamos crear una función que le de valores a una variable `COMPREPLY`. Esta función se llamará cuando apretemos *Tab*, y deberá agregar las sugerencias en este array COMPREPLY. Además, cuando lo llamemos habrá una variable `COMP_WORDS` que será un array de las palabras que venimos tipeando.

Para probar como funciona escribiremos la siguiente función en un archivo `function-autocomplete.sh`.

~~~
_some_app() {
    echo "COMP_WORDS: ${COMP_WORDS[*]}"
    COMPREPLY=("COMP1" "COMP2" "COMP3" "FOO")
}

complete -F _some_app ./some-app
~~~

Corremos este script con `source function-autocomplete.sh` para cargarlo en nuestra terminal.

Cuando escribimos `./some-app` y apretamos tab por primera vez podemos ver que se llama a la función. Cuando volvemos a apretar vemos las sugerencias que agregamos en COMPREPLY.

Notar que si escribimos, por ejemplo `./some-app CO` e intentamos autocompletar, no solo se llena COMP_WORDS con el nuevo pedacito de sugerencia, sino que tambien aparecen las sugerencias iniciales, pero sin filtrar.

Para filtrar las sugerencias que agregamos programáticamente usamos el comando `compgen`. Este comando es muy similar a `complete`, pero a diferencia de este puede recibir como parámetro el texto sobre el que queremos autocompletar. Utiliza este texto para filtrar las opciones sugeridas.

Cambiamos nuestra función a lo siguiente:  

~~~
_some_app() {
    COMPREPLY=($(compgen -W "COMP1 COMP2 COMP3 FOO" "${COMP_WORDS[1]}"))
}

complete -F _some_app ./some-app
~~~

Ahora podemos filtrar según la primer palabra.

Es un acercamiento bastante útilm aunque no nos sirve si queremos agregar más parámetros

### init_completion

Bash nos provee de una función `_init_completion` que nos carga varias variables de utilidad.

~~~
local cur prev words cword
_init_completion || return
~~~

Donde `cur` es la palabra actual, `prev` es la anterior, `words` es la lista de palabras y `cword` la cantidad de palabras.

Podemos probar que con el siguiente cambio nos permite autocompletar varios argumentos de los siguientes

~~~
_some_app() {
    COMPREPLY=($(compgen -W "COMP1 COMP2 COMP3 FOO" "$cur"))
}
~~~

Volviendo a nuestro ejemplo original

~~~
_some_app() {
    local cur prev words cword
    _init_completion || return
    COMPREPLY=($(compgen -W "-v -i -o --input --output" -- "$cur"))
}
~~~

Agregamos los 2 guiones entre compgen y sugerencia para que no compgen no tome a $cur como un flag propio.


## Juntando todo lo visto

Vamos a juntar todo lo visto para hacer un autocompletado que:

Si la opción elegida es `-i` o `--input` sugiera archivos `.sh` para autocompletar.

Si la opción elegida es `-o` o `--output` sugiera directorios para autocompletar.

Si se encuentra la opión `-v`, que sugiera las otras opiones

Si no es ninguna opción, que sugiera la lista de opciones.

Empezamos armando un bloque *switch-case*: La sintaxis como siempre es muy... extraña, pero no es mi intención centrarme en bash.

~~~
_some_app() {
    local cur prev words cword
    _init_completion || return
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
    COMPREPLY=($(compgen -W "-v -i -o --input --output" -- "$cur"))
}

complete -F _some_app ./some-app
~~~

Ya con este script podemos cambiar las sugerencias según las opciones. Sin embargo, como sólo verifico cual fue la última opción podemos, por ejemplo, repetir el flag -v alternándolo con otros flags.

Para esto usamos una variable que nos ayude a encontrar que está usado.

### Buscando entre las palabras ya ingresadas

Iteramos sobre el array `words` buscando `-v`, si lo encontramos, desactivamos este flag.

~~~
local VERBOSE_FLAG="-v"
for word in "${words[@]}"
do
    if [ "$word" -eq "-v" ] ; then
        VERBOSE_FLAG=""
    fi
done
~~~

Y nuestro caso default será el siguiente:

~~~
COMPREPLY=($(compgen -W "$VERBOSE_FLAG -i -o --input --output" -- "$cur"))
~~~

Ahora nuestro autocompletado va variando en función de los flags que encuentra.

# Instalación

Los scripts de autocompletado suelen ubicarse en la carpeta `/usr/share/bash-completion/completions/`. Se sugiere agregarlo en el paquete de instalación.

**NOTA**: Para probar el script estuvimos usando como ruta `./some-app`, si se lo instala en el sistema, debería tener únicamente el nombre de la aplicación para que se pueda invocar desde cualquier parte.

# Bibliografía

* https://www.hiroom2.com/2017/05/04/linux-complete-original-command-argument-with-bash-completion/

* https://iridakos.com/programming/2018/03/01/bash-programmable-completion-tutorial
