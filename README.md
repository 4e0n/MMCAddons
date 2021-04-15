# MMCUtility
Moodle Multiple Choice Exam Preparation Utility

This is a simple utility I have written according to our needs in N.E.U. Meram Medical Faculty for easily preparing an examination offline and single-attempt uploading to a Moodle Exam Server. We typically accept a number of questions from several departments for a general examination. The questions are normally needed to be entered to the system online, and it is both slow, in terms of page switches, and also has a cumbersome interface. This utility provides the operator a much faster and pratical user interface specialized for only homogeneous multiple choice exams.

It basically accepts a predefined number of questions via separate fields (fitted for fast copy-paste operation from e.g. a word file). Formulas and other mathematical support is over LaTeX support. Currently, adding a single .png or .jpg figure to the question body is possible. In the future, figure support for multiple-choice answers is planned.

Figures are encoded to BASE64 and suitably embedded to the destination Moodle XML file, which is typically generated after all questions are prepared.

I wrote it fast for a quick need during pandemic conditions so it is currently in Turkish, but I have plans to provide English and French support in the following weeks.
