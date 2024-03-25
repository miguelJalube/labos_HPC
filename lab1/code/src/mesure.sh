if [ ! -x "./lab01" ]; then
    echo "Le programme 'lab01' n'existe pas dans ce répertoire ou n'est pas exécutable."
    exit 1
fi

echo "Exécution pour image 1D :"
/usr/bin/time -f "%e seconds" ./lab01 $1 $2 1

echo "Exécution pour image chained :"
/usr/bin/time -f "%e seconds" ./lab01 $1 $2 2
