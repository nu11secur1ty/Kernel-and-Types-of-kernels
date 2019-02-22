This is a Linus Torvald's first rule of kernel development. This note explains it: https://lkml.org/lkml/2012/12/23/75. I.e., when maintaining the kernel, do not do something which breaks user programs/applications. In other words, when making kernel changes, it is very bad to cause problems in the user's application "space". That doesn't literally mean memory. That means anything that impacts the user applications in a way that negatively affects its behavior (causes the program to malfunction). The note I cite also indicates at least one example.


-----------------------------------------------------

```bash
From	Linus Torvalds <>
Date	Sun, 23 Dec 2012 09:36:15 -0800
Subject	Re: [Regression w/ patch] Media commit causes user space to misbahave (was: Re: Linux 3.8-rc1)
On Sun, Dec 23, 2012 at 6:08 AM, Mauro Carvalho Chehab
<mchehab@redhat.com> wrote:
>
> Are you saying that pulseaudio is entering on some weird loop if the
> returned value is not -EINVAL? That seems a bug at pulseaudio.

Mauro, SHUT THE FUCK UP!

It's a bug alright - in the kernel. How long have you been a
maintainer? And you *still* haven't learnt the first rule of kernel
maintenance?

If a change results in user programs breaking, it's a bug in the
kernel. We never EVER blame the user programs. How hard can this be to
understand?

To make matters worse, commit f0ed2ce840b3 is clearly total and utter
CRAP even if it didn't break applications. ENOENT is not a valid error
return from an ioctl. Never has been, never will be. ENOENT means "No
such file and directory", and is for path operations. ioctl's are done
on files that have already been opened, there's no way in hell that
ENOENT would ever be valid.

> So, on a first glance, this doesn't sound like a regression,
> but, instead, it looks tha pulseaudio/tumbleweed has some serious
> bugs and/or regressions.

Shut up, Mauro. And I don't _ever_ want to hear that kind of obvious
garbage and idiocy from a kernel maintainer again. Seriously.

I'd wait for Rafael's patch to go through you, but I have another
error report in my mailbox of all KDE media applications being broken
by v3.8-rc1, and I bet it's the same kernel bug. And you've shown
yourself to not be competent in this issue, so I'll apply it directly
and immediately myself.

WE DO NOT BREAK USERSPACE!

Seriously. How hard is this rule to understand? We particularly don't
break user space with TOTAL CRAP. I'm angry, because your whole email
was so _horribly_ wrong, and the patch that broke things was so
obviously crap. The whole patch is incredibly broken shit. It adds an
insane error code (ENOENT), and then because it's so insane, it adds a
few places to fix it up ("ret == -ENOENT ? -EINVAL : ret").

The fact that you then try to make *excuses* for breaking user space,
and blaming some external program that *used* to work, is just
shameful. It's not how we work.

Fix your f*cking "compliance tool", because it is obviously broken.
And fix your approach to kernel programming.

               Linus
```
