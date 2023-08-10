#!/bin/bash

#cria diretorio em nobackup
cd /nobackup/bcc/vrg20
if [ -d "156" ]
then
        echo "diretorio 156 já existente"
else
        mkdir 156
fi
cd 156

#download do código fonte
if [ -e "index.html" ]
then
        echo "index.html já existe"
else
        index.html:
        wget http://dadosabertos.c3sl.ufpr.br/curitiba/156/
fi

#filtrando urls ano 2021 que queremos e salvando em um arquivo txt
URLS=$(grep "<a href=" index.html | cut -d"=" -f5 | cut -d">" -f1 | tr -d '"' | grep -v "Historico" | grep "2021-..-.._156_-_.*" )

echo "$URLS" > urls.txt

#extraindo arquivos .csv e removendo arquivo txt
if [ -d "CSV" ]
then
        echo "arquivos .csv ja extraidos"
else
        mkdir CSV
        while IFS= read -r line
        do
                wget dadosabertos.c3sl.ufpr.br/curitiba/156/$line
                mv 2021* CSV
        done < urls.txt
fi

#convertendo arquivos para sistemas unix
ConverteUTF8(){
        while IFS= read -r line
        do
                dos2unix ./CSV/$line
                sed -i 's/\x00/ /g' ./CSV/$line
                iconv -f ISO-8859-1 -t UTF8 ./CSV/$line -o ./CSV/$line
                file ./CSV/$line
                echo "conversão para UTF8 realizada com sucesso"
        done < urls.txt
}
ConverteUTF8
rm urls.txt

#obter ASSUNTOS e SUBDIVISOES
cd CSV
cat 2021-* | cut -d";" -f6 >> assunto_prov.txt
cat 2021-* | cut -d";" -f7 >> subdivisao_prov.txt
cat assunto_prov.txt | sort -u > assunto.txt
rm assunto_prov.txt
cat subdivisao_prov.txt | sort -u > subdivisao.txt
rm subdivisao_prov.txt

cd


