Chitr for macOS
===============

This build is not signed with an Apple Developer ID, so macOS Gatekeeper
will refuse to open it the first time with a message like:

    "Chitr" cannot be opened because it is from an unidentified developer.

You have two ways to run it:

Option A — one-time Terminal command (recommended)
---------------------------------------------------
Open Terminal, cd to the folder where you unzipped this, and run:

    xattr -dr com.apple.quarantine Chitr.app

Then double-click Chitr.app as normal. You only need to do this once.

Option B — right-click the app
------------------------------
In Finder, right-click (or Control-click) Chitr.app and choose "Open".
Confirm the dialog. macOS remembers this choice.

Why is this needed?
-------------------
macOS blocks downloaded apps that are not signed and notarized by Apple.
Signing and notarization require a paid Apple Developer account. If you
would rather not run either of the commands above, you can build Chitr
from source instead — see Readme.md in this archive, or the project page.
