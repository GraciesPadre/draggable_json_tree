# draggable_json_tree

I built this on CentOS 7 using the Qt 5 development package gotten with yum install.

To build, you'll need Cmake 3.10 or later.  You can either build by using Cmake directly
or follow the path of least resistance and get CLion, which comes with an evaluation license...

https://www.jetbrains.com/clion/specials/clion/clion.html?gclid=EAIaIQobChMIhvmz-cbn4AIVCJ7ACh2tqwCKEAAYASAAEgK5d_D_BwE&gclsrc=aw.ds

This package builds 2 executables: json_tree, the app, and json_tree_test.  The test
executable will automatically download and build gtest master.