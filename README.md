# gse-command-line

This group of command-line tools was constructed by Steve McBride prior to the FOXSI-1 launch.  As of the initial creation of this GitHub repository, no changes have been made to McBride's original programs.  For both reading data and commanding, there are GUI applications available (FOXSIGSE and FOXSI COMMANDER).  However, these command line programs are often useful.

Info, syntax, examples:

SETHV:  Command the formatter to set the bias voltage (HV).  The value input for the bias voltage should be volts * 8.

./sethv <devicename> <HV*8>
  
ACTELCMD:  Send commands to the detector FPGAs.  These were not used in the FOXSI-1 and FOXSI-2 flight.  Use at your own risk.  For a full description of the commands that are available and how to use them, see the document "commanding notes.pdf" in the FOXSI Google Drive.

./actelcmd <devicename> <detector> <command>
  
ATTENUATOR0 and ATTENUATOR1:  These commands prompt the attenuator.  Currently we have no way to remove the attenuator, so insertion is the only possible action.

./attenuator1 <devicename> 1

  TO DO: Check to see the effect of each attenuator0 and attenuator0

FRAMECHECK and FRAMECHECKN:  These programs do not communicate with the FOXSI detector system.  Rather, they check an acquired formatter data file for the number of good frames.  FRAMECHECK does this for a binary file acquired through the FOXSIGSE formatter interface, while FRAMECHECKN does this for the .LOG files that are recorded for us by the WSMR Ground Station.

GSETEST through GSETEST3:  These programs are used for acquiring data through the formatter interface.  GSETEST3 is the most developed and is the one typically used for data acquisition.
