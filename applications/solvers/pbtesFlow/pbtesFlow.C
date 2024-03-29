/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 Johan Roenby & Jakob Hærvig
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    porousPimpleFoam

Description
    Solver for incompressible flow through a porous medium with which the fluid
    exchanges heat.

Authors
    Johan Roenby, Roskilde University and STROMNING APS
    Jakob Hærvig, Aalborg University

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "dynamicFvMesh.H"
//#include "isoAdvection.H"
//#include "EulerDdtScheme.H"
#include "localEulerDdtScheme.H"
//#include "CrankNicolsonDdtScheme.H"
//#include "subCycle.H"
//#include "immiscibleIncompressibleTwoPhaseMixture.H"
//#include "incompressibleInterPhaseTransportModel.H"
#include "pimpleControl.H"
#include "fvOptions.H"
#include <CorrectPhi.H>
//#include "fvcSmooth.H"
//#include "dynamicRefineFvMesh.H"
#include "upwind.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "Solver derived from interIsoFoam for incompressible flow through"
        "a porous medium with which it exchanges heat."
    );

    #include "postProcess.H"

    #include "addCheckCaseOptions.H"
    #include "setRootCaseLists.H"
    #include "createTime.H"
    #include "createDynamicFvMesh.H"
    #include "initContinuityErrs.H"
    #include "createDyMControls.H"
    #include "createFields.H"
    #include "initCorrectPhi.H"
    #include "createUfIfPresent.H"

    #include "porousCourantNo.H"
    #include "setInitialDeltaT.H"

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
        #include "readDyMControls.H"
        #include "porousCourantNo.H"
        #include "setDeltaT.H"

        ++runTime;

        Info<< "Time = " << runTime.timeName() << nl << endl;

        // --- Pressure-velocity PIMPLE corrector loop
        while (pimple.loop())
        {
            if (pimple.firstIter() || moveMeshOuterCorrectors)
            {

                mesh.update();

                if (mesh.changing())
                {
                    MRF.update();

                    if (correctPhi)
                    {
                        // Calculate absolute flux
                        // from the mapped surface velocity
                        phi = mesh.Sf() & Uf();

                        #include "correctPhi.H"

                        // Make the flux relative to the mesh motion
                        fvc::makeRelative(phi, U);

                    }

                    if (checkMeshCourantNo)
                    {
                        #include "meshCourantNo.H"
                    }
                }
            }

            if (pimple.frozenFlow())
            {
                continue;
            }

            #include "UEqn.H"

            // --- Pressure corrector loop
            label nCorr = 0;
            while (pimple.correct())
            {
                #include "pEqn.H"
                nCorr++;
            }

            #include "TEqn.H"
        }

        runTime.write();

        runTime.printExecutionTime(Info);
    }

    Info << "End\n" << endl;

    return 0;
}


// ************************************************************************* //
