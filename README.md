# Compare Regex

This is a simple script to compare multiple regex engines and compare their performance on the same strings and regex patterns. It borrows code from [waflz](https://github.com/Edgio/waflz) for pcre1 and I adapted the pcre2 and hyperscan code from that using various sources on the Internet.

To run the script, you can run the the following command:
```bash
./run.sh
```

I built this on MacOS so you will need to have pcre1, pcre2, and hyperscan installed on your machine. You can install them using brew:
```bash
brew install pcre
brew install pcre2
brew install hyperscan
```