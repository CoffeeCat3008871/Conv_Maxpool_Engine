#pragma once

const float filterFactors[] = {
    1.0/9,
};

const short filterBiases[] = {
    0,              // Identity
};

const char filterCoeffs[][3][3] = {    
    {
        // filterCoeffs[0] -> Edges
        {1,1,1},
        {1,1,1},
        {1,1,1},
    },             
};    
