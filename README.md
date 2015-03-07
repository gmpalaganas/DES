###Data Encryption Standard Emulation

Takes an input file named "input.txt" then outputs a file named "201232936.txt"


The input file lines have the format:

1 encrypt {i<sub>1</sub>,i<sub>2</sub>,i<sub>3</sub>,i<sub>4</sub>,i<sub>5</sub>,i<sub>6</sub>,i<sub>7</sub>,i<sub>8</sub>}{k<sub>1</sub>,k<sub>2</sub>,k<sub>3</sub>,k<sub>4</sub>,k<sub>5</sub>,k<sub>6</sub>,k<sub>7</sub>,k<sub>8</sub>}

or

2 decrypt {i<sub>1</sub>,i<sub>2</sub>,i<sub>3</sub>,i<sub>4</sub>,i<sub>5</sub>,i<sub>6</sub>,i<sub>7</sub>,i<sub>8</sub>}{k<sub>1</sub>,k<sub>2</sub>,k<sub>3</sub>,k<sub>4</sub>,k<sub>5</sub>,k<sub>6</sub>,k<sub>7</sub>,k<sub>8</sub>}


Where i is the plaintext, k is the key and i<sub>n</sub> and k<sub>n</sub> are 64-bit integers (0 - 255) 


The output file line have the format

1 c<sub>1</sub>,c<sub>2</sub>,c<sub>3</sub>,c<sub>4</sub>,c<sub>5</sub>,c<sub>6</sub>,c<sub>7</sub>,c<sub>8</sub>


Where where c is the ciphertext and c<sub>n</sub> are 64-bit integers (0 - 255) 
