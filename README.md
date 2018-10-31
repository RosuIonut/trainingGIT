# Welcome to the JGP project!

This is the source code repository of the JGP software platform project of ID RD SW.
 
Key features:
* GreenHills INTEGRITY OS with ACE OS wrapper and 
* GreenHills board support package (BSP) as hardware driver layer
* multi process environment
* Freescale i.MX6 CPU with dual core configuration, build variants: sabre auto, W213es2, W222mopf
* Renesas HL3D CPU, build variants: D1H1 merlot, D1H2 merlot, HUD2G_Redesign
* this JGP software platform also provides the software part of the JCP2016 HL3D chip


## links
* this source repository in [git](http://git-id.conti.de/cds/sw.sys.jgp_ref)
* documents repository in [git](http://git-id.conti.de/cds/sw.com.jgp2014_doc)
* project network share \\\\cw01\\root\\Loc\\bbuv\\did35819\\_JGP2014
* issue tracking [jira](http://bhs0jira.cw01.contiwan.com)
* project [sharepoint](https://cws1.conti.de/content/00011136/JGP2014/Forms/AllItems.aspx)
* the BSP is available in the form of libraries in this repo only. BSP sources (restricted access) are tracked in other repositories. Ask department ID RD SW OA O for access rights.
* flash boot loader repository [git repo](http://git-id.conti.de/cds/sw.sys.jgp_fbl)
* confluence [wiki](http://wiki-id.conti.de/display/JGP)
* JGP [blog](http://wiki-id.conti.de/pages/viewrecentblogposts.action?key=JGP)
* SE tool web http://info5.conti.de/app6/passengerCars/SW/tools/start.htm

### continuous integration
* for integration strategy, branch ownership and commit rules read [this](http://git-id.conti.de/cds/sw.com.jcp_concept/blob/master/02_Integration_Strategy/20_des/Contribution_Strategy.pptx?raw=true)
* develop branch (http://bhs001x.bh.de.conti.de:8080/jenkins/view/JGP/view/develop/)
* wallboard view (http://bhs001x.bh.de.conti.de:8080/jenkins/view/monitor/view/JGP/)
* CDS [recommendations](doc/continuous_integration.md) for setting up CI jobs in customer projects

## documents
* [getting started](doc/startmeup.md). Read this first if you are new to the project and want to build the system.
* documentation for the SQE IDE scripts environment can be found at ide\doc
* [Integrated Development Environment](doc/ide.md)
* [Toolchain](doc/toolchain.md)
* fasten tool runtime by the [pre generated outputs archive functionality](http://wiki.bh.de.conti.de/display/TOOL/Tool+cache+functionality)
* [arbor](http://wiki-id.conti.de/display/ARBOR/User+Manual)
* [cmake_structure](http://wiki-id.conti.de/display/TOOL/CMAKE+Structure)
* [multi_python_scripting](doc/multi_python_scripting.md)
* [code generation tool requirements] (http://wiki-id.conti.de/display/TOOL/Best+practices+for+code+generation+tools)
* [simulation+target builds co-exist] (http://wiki-id.conti.de/display/TOOL/Simulation+and+Target+builds+co-exist+in+the+same+work+folder)


## about
> This README.md is written in [markdown](http://en.wikipedia.org/wiki/Markdown). Use the git link above to easily browse through all files.
A full explanation of github's render capabilities and the syntax is [described here](https://help.github.com/articles/github-flavored-markdown/).
This README.md is tracked as any other file in this source repository.

## CIPS BringUp HUD2GEN
- Check Artifact Drive
- Check JIRA connection (CIPS Test Project)
- Check ICOM Generation
- fDarling Variant Check
- another Jenkins test
- 3'rd fDarling Variant Check
- retrigger Jenkins to update BVT
- no code change. trigger jenkis to change jira ticket status
- GVK_1622 fakechange
- GVK_1341 fakechange to reintegrate
- GVK_1772 fakechange
- GVK_2287 fakechange
- GVK-3683 CIPSv01.03.01 2nd test
- GVK-3382 - GS update made for GVK-15 (already integrated). -> close the ticket.
- GVK-7626 - fake change to reintegrate
- GVK-7478: fake change to reintegrate
- GVK-7982: fake change to reintegrate
 