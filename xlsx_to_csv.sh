#!/usr/bin/env bash

for f in google_sheets/*.xlsx
do
    echo $f
    libreoffice --headless --convert-to csv $f --outdir csv_files 

    # alternative file converting method
    # ssconvert "$f" "${f%.xlsx}.csv"
done
