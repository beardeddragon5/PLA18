#!/bin/bash

function faillex() {
  local testExit=$?
  local test=$1
  local output=$2

  local expected=0
  if case $test in ../tests/faillex*) ;; *) false;; esac; then
    expected=1
  fi

  if [ $testExit -ne $expected ]
  then
    cat test_out.txt
    echo "Failed: $test"
    exit 1
  fi
}

function failparse() {
  local testExit=$?
  local test=$1
  local output=$2

  local expected=0
  if case $test in ../tests/fail*) ;; *) false;; esac; then
    expected=1
  fi

  if [ $testExit -ne $expected ]
  then
    echo $output
    echo "Failed: $test"
    exit 1
  else
    echo "Success: $test"
  fi

}

make lexmain
make parsemain

tests=($(ls ../tests/*))
for test in ${tests[@]}
do
  echo "Test: $test"
  ./lexmain $test - &> test_out.txt
  faillex "$test"

  ./parsemain $test - &> test_out.txt
  failparse "$test"

  echo
done

rm test_out.txt
