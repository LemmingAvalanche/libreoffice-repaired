# Repaired Libreoffice Repository

Bad commit objects have been fixed by rewriting (`git filter-repo
--force`).

Bad commit metadata caused weird git-log(1) behavior.

This forcibly rewritten repository is just a reference in order to
demonstrate that git-log(1) works as long as the commit objects are
sound (not bad). So there is no bug in git-log(1).

# Context

See email thread [“Weird behavior of 'git log
--before'”](https://lore.kernel.org/git/7728e059-d58d-cce7-c011-fbc16eb22fb9@cs.uni-saarland.de/)
on the Git mailing list.

This was provided for reproduction:

    # (Note by me: might wanna use HTTPS instead)
    git clone git@github.com:LibreOffice/core.git libreoffice
    cd libreoffice
    git log --no-merges --before="1980-01-01T00:00:00+0000" --format=%H,%ct,%ci,%ad

And indeed this works as a way to reproduce the weird behavior.

But this behavior can no longer be reproduced on this repository, where
the bad commits have been fixed.

Proof:

    git clone https://github.com/LemmingAvalanche/libreoffice-repaired
    cd libreoffice-repaired
    git log --no-merges --before="1980-01-01T00:00:00+0000" --format=%H,%ct,%ci,%ad

The `git log` invocation should give no output (the repository does not
go back to the 1980’s).

# Bad commits: output from `git fsck --strict`

Example of commits in the [original
repo](https://github.com/LibreOffice/core) that seem to foil git-log(1):

    error in commit 08b6b244b215ec4a96686866685eab5421da3dd5: missingSpaceBeforeEmail: invalid author/committer line - missing space before email
    error in commit 3da23a64509eb47f9bf56c65611d11a64b89583b: missingSpaceBeforeDate: invalid author/committer line - missing space before date

# Repaired metadata

These were changed because they were completely mangled:

    andre.f.fischer <Andre Fischer<andre.f.fischer@oracle.com → Andre Fischer <andre.f.fischer@oracle.com>
    Andre.W.Fischer <Andre Fischer<Andre.W.Fischer@Sun.COM → Andre Fischer <Andre.W.Fischer@Sun.COM>
    gh <Gregor Hartmann<gh@openoffice.org → Gregor Hartmann <gh@openoffice.org>
    Joerg Skottke [jsk] jsk@openoffice.org → Joerg Skottke <jsk@openoffice.org>

And that was enough to make git-log(1) stop being weird.
