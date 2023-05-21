Usually CTest is used in combination with another testing framework such as Boost or GTest. Since
here at Nichess we don't tolerate bloat, we use CTest alone. One drawback of this is that it
requires creating a separate file for each test, which makes writing and maintaing a large number of
tests difficult. Fortunately, this problem can be solved by creating a test runner executable, as
explained [here](https://baoilleach.blogspot.com/2013/06/using-ctest-with-multiple-tests-in.html).
