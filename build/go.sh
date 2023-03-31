#!/bin/bash
cmake .. && make && cp compile_commands.json .. && cp tuxbrawler .. && cd .. && ./tuxbrawler
