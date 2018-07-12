# gse-command-line

This group of command-line tools was constructed by Steve McBride prior to the FOXSI-1 launch.  As of the initial creation of this GitHub repository, no changes have been made to McBride's original programs.  For both reading data and commanding, there are GUI applications available (FOXSIGSE and FOXSI COMMANDER).  However, these command line programs are often useful.

Info, syntax, examples:

SETHV:  Command the formatter to set the bias voltage (HV).  The value input for the bias voltage should be volts * 8.

./sethv <devicename> <HV*8>
  
ACTELCMD:  Send commands to the detector FPGAs.  These were not used in the FOXSI-1 and FOXSI-2 flight.  Use at your own risk.  For a full description of the commands that are available and how to use them, see the document "commanding notes.pdf" in the FOXSI Google Drive.

./actelcmd <devicename> <detector> <command>
  
ATTENUATOR0 and ATTENUATOR1:  These commands prompt the attenuator.  Currently we have no way to remove the attenuator, so insertion is the only possible action.

FRAMECHECK and FRAMECHECKN:  This program does not communicate with the FOXSI detector system.  Rather, it checks an acquired formatter data file for the number of good frames.

GSETEST through GSETEST3:  These programs are used for acquiring data through the formatter interface.  GSETEST3 is the most developed and is the one typically used for data acquisition.
