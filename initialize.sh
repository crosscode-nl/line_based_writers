#!/bin/bash

echo "Project name: "
read PROJECT_NAME
echo "Package name: "
read PROJECT_PACKAGE_NAME

PROJECT_NAME_UPPER=${PROJECT_NAME^^}

rm -rf .git

sed -i 's/cpp_project_template_package_name/'"${PROJECT_PACKAGE_NAME}"'/g' README.md
sed -i 's/cpp_project_template/'"${PROJECT_NAME}"'/g' README.md

sed -i 's/cpp_project_template/'"${PROJECT_NAME}"'/g' include/cpp_project_template.h
sed -i 's/CPP_PROJECT_TEMPLATE/'"${PROJECT_NAME_UPPER}"'/g' include/cpp_project_template.h

mv include/cpp_project_template.h include/"${PROJECT_NAME}".h
mv include/cpp_project_template include/"${PROJECT_NAME}"

mv src/cpp_project_template src/"${PROJECT_NAME}"

sed -i 's/cpp_project_template/'"${PROJECT_NAME}"'/g' appveyor.yml

sed -i 's/cpp_project_template_package_name/'"${PROJECT_PACKAGE_NAME}"'/g' CMakeLists.txt
sed -i 's/cpp_project_template/'"${PROJECT_NAME}"'/g' CMakeLists.txt

sed -i 's/cpp_project_template/'"${PROJECT_NAME}"'/g' tests/version_tests.cpp

sed -i 's/cpp_project_template_tests/'"${PROJECT_NAME}_tests"'/g' tests/CMakeLists.txt

sed -i 's/cpp_project_template/'"${PROJECT_NAME}"'/g' tests/cpp_project_template_tests.cpp
mv tests/cpp_project_template_tests.cpp tests/"${PROJECT_NAME}"_tests.cpp

sed -i 's/cpp_project_template/'"${PROJECT_NAME}"'/g' examples/example01.cpp

git init
git checkout -b main
git add .
git commit -m "Initial commit."
