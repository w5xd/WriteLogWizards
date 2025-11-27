The files here were carried over from the vsz-based sources in WL12ModuleWizard\Templates\1033
Originally I thought I would retired the vsz-based versions in favor of the new vstemplate versions.
Upon further review, I changed my mind. 
The ConvertToVsTemplate program had to be designed because of features in the "retired" system that
are not in the new: 
    nested ifdef
    ability to form general case logic of && and ||. The new system can only do &&
Those missing features make the maintenance of the sources far harder. 

So I am not going to retire the old templates. They are here so that they can be changed 
and then ConvertToVsTemplate can be run again