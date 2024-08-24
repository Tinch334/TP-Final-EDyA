make sensor -s && \
mkfifo feedback && \
(./out < feedback | ./sensor $1 > feedback ; \
rm feedback)