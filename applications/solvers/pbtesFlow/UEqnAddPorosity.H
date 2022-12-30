// Including porosity effects in UEqn

{
    const word porosityModel("darcyForchheimerPorosity");
    const dictionary& dict =
        porosityProperties.subDict(porosityModel + "Coeffs");
    const dimensionedScalar alpha(dimless/dimArea, dict.get<scalar>("alpha"));
    const dimensionedScalar beta(dimless/dimLength, dict.get<scalar>("beta"));
    const dimensionedScalar dpp(dimless, dict.getOrDefault<scalar>("darcyPorosityPower",0.0));
    const dimensionedScalar d1mpp(dimless, dict.getOrDefault<scalar>("darcyOneMinusPorosityPower",1.0));
    const dimensionedScalar fpp(dimless, dict.getOrDefault<scalar>("ForchheimerPorosityPower",0.0));
    const dimensionedScalar f1mpp(dimless, dict.getOrDefault<scalar>("ForchheimerOneMinusPorosityPower",1.0));

    // Generating Darcy-Forchheimer coefficient for force term of form: 
    // F = alpha*muf*n^dpp*(1-n)^d1mpp*U + beta*rhof*n^fpp*(1-n)^f1mpp*|U|*U
    // where mu is viscosity and n is porosity.
    {
        // Darcy term
        volScalarField DarcyForchheimerCoeff
        (
            alpha*muf*pow(porosity, dpp)*pow(1 - porosity, d1mpp)
        );

        // Adding Forchheimer term
        DarcyForchheimerCoeff += beta*rhof*mag(U)*pow(porosity, fpp)*pow(1-porosity, f1mpp);

        // Adding Darcy-Forchheimer term as implicit source term
        UEqn += fvm::Sp(DarcyForchheimerCoeff, U);
    }

    {
        // Generating added mass force coefficient
        const dimensionedScalar gamma(dimless, dict.getOrDefault<scalar>("gamma",0.0));
        const volScalarField Cm(gamma*(1 - porosity));

        UEqn += Cm*fvm::ddt(rhof, U);
    }

    // Dividing both matrix entries and source term by porosity to to compensate
    // for the use of cell volume instead of cell pore volume and face area
    // instead of face pore area in the discretisation schemes.
    UEqn *= scalar(1)/porosity;
}
