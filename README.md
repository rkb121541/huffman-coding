# Hzip

I use the Huffman Coding algorithm to make a file compressor software. I would say it is less of a software and more of a simulator since it does not actually store the file in a binary/ more efficient format. I actually tried storing the encodings into bits and into a binary file, but it got complicated. Instead, you can view the human readable versions of the encoded files in the `./examples_encodings/` directory. It would be stored similarly if I was storing it in a binary file except I would also have to add the sizes and lengths of the encodings and mappings before writing it to the file.

In my implementation of the Huffman Coding algorithm, I order the priority queue from min to max.

Since I use `std::format()` which is only available in C++ versions 20 and above, I have to compile `main.cpp` by running `g++ -std=c++20 main.cpp`.

You can run the program by compiling it:
```sh
g++ -std=c++20 main.cpp
```

And then run the executable:
```sh
./a.out
```

The `./examples_og/` directory contains some examples of txt files that we can use to perform the huffman coding algorithm on. You can also put your own files in this directory and run the program.

The program will prompt you to enter the name of file, but will look for it specifically in the `./examples_og/` directory. So make sure it is there or else it will throw an error. You can also just run it on the existing files there. But I already ran all of them, so you can just view the encoded and decoded versions of their files in their respective directories (`./examples_encodings/` and `./examples_decodings/`).

The `./examples_encodings/` directory provides encodings of these texts respectively. It already uses the prefix of the file before the .txt to name the file so you know which one is which.

You are only allowed to put `.txt` files into the `./examples_og/` directory. I have not tested it with any other types of files and it may cause problems.