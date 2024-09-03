#If file "feedback" exists delete it.
if [ -f feedback ]; then
    rm feedback
fi

make sensor -s && \
mkfifo feedback && \
(./out < feedback | ./sensor $1 > feedback ; \
rm feedback)