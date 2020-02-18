#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2019 weirdo <weirdo@Hack-Box>
#
# Distributed under terms of the MIT license.

"""

"""
import random

ind = 1
with open("WikiTalk.txt") as f:
    with open("WikiTalk_gabow.txt", "w") as f1:
        for line in f:
            temp = line
            if ind != 1:
                temp = temp + str(random.randrange(1, 1000, 1)) + '\n'
                #print(temp)
            ind = ind + 1
            f1.write(temp)
