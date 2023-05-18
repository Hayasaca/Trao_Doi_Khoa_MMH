//  Defines the entry point for the console application
/*ECC parameters p,a,b, P (or G), n, h where p=h.n*/

/* Source, Sink */
#include "filters.h"

#include <ctime>
#include <iostream>
#include <string>
using namespace std;

/* Randomly generator*/
#include "osrng.h"
using CryptoPP::AutoSeededRandomPool;

/* Integer arithmatics*/
#include <integer.h>
using CryptoPP::Integer;
#include <nbtheory.h>
using CryptoPP::ModularSquareRoot;

#include <ecp.h>
#include <eccrypto.h>
using CryptoPP::ECP;    // Prime field p
using CryptoPP::ECIES;
using CryptoPP::ECPPoint;
using CryptoPP::DL_GroupParameters_EC;
using CryptoPP::DL_GroupPrecomputation;
using CryptoPP::DL_FixedBasePrecomputation;

#include <pubkey.h>
using CryptoPP::PublicKey;
using CryptoPP::PrivateKey;

/* standard curves*/
#include <asn.h>
#include <oids.h> // 
namespace ASN1 = CryptoPP::ASN1;
using CryptoPP::OID;


int main(int argc, char* argv[])
{
    AutoSeededRandomPool rng;
    // Contruct  ECP(const Integer &modulus, const FieldElement &A, const FieldElement &B);

            // User Defined Domain Parameters for curve y^2 =x^3 + ax +b
            // Modulus p
    Integer p("fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffeffffffff0000000000000000ffffffffh");
    // Coefiction a
    Integer a("fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffeffffffff0000000000000000fffffffch");
    // Coefiction b
    Integer b("b3312fa7e23ee7e4988e056be3f82d19181d9c6efe8141120314088f5013875ac656398d8a2ed19d2a85c8edd3ec2aefh");
    /* create a curve*/
    a %= p;     b %= p; // a mod p, b mod p
    /* ECC curve */
    CryptoPP::ECP eqcurve384(p, a, b); // buide curve y^2 =x^3 +ax +b
    /* subgroup <G> on curve */
     // x, y: Base Point G
    Integer x("aa87ca22be8b05378eb1c71ef320ad746e1d3b628ba79b9859f741e082542a385502f25dbf55296c3a545e3872760ab7h");
    Integer y("3617de4a96262c6f5d9e98bf9292dc29f8f41dbd289a147ce9da3113b5f0b8c00a60b1ce1d7e819d7a431d7c90ea0e5fh");
    // Creat point G
    ECP::Point G(x, y);
    // Oder n of group <G>
    Integer n("ffffffffffffffffffffffffffffffffffffffffffffffffc7634d81f4372ddf581a0db248b0a77aecec196accc52973h");
    //Cofactors
    Integer h("01h");
    /* Set ECC parameters and subgroup <G>*/
    // CryptoPP::DL_GroupParameters_EC<ECP> curve256(eqcurve256,G,n,h);
    CryptoPP::DL_GroupParameters_EC<ECP> curve384;
    curve384.Initialize(eqcurve384, G, n, h);
    /* Get curve paramaters p, a, b, G, n, h*/
    cout << "Cofactor h=" << curve384.GetCofactor() << endl;
    cout << "Subgroup Order n=" << curve384.GetSubgroupOrder() << endl;
    cout << "Gx=" << curve384.GetSubgroupGenerator().x << endl;
    cout << "Gy=" << curve384.GetSubgroupGenerator().y << endl;
    cout << "Coefficient  a=" << curve384.GetCurve().GetA() << endl;
    cout << "Coefficient  b=" << curve384.GetCurve().GetB() << endl;
    //cout <<"Prime number p=" <<curve384.GetCurve().GetField()<<endl;
    /* Computation on Curve Add, double, scalar mutiplication*/
    ECP::Point Q = curve384.GetCurve().Double(G); // G+G;
    /*cout << "Qx=" << Q.x << endl;
    cout << "Qy=" << Q.y << endl;*/
    Integer r("615238761874518718411432514351734513434133154432"); // Thay bang gia tri tu chon 
    r %= p;
    //cout << "number r=" << r << endl;
    ECP::Point H = curve384.GetCurve().ScalarMultiply(G, r); // rP;
    //cout << "Hx=" << H.x << endl;
    //cout << "Hy=" << H.y << endl;
    ECP::Point I = curve384.GetCurve().Add(Q, H); // Q+H=2G+6000G
    //cout << "Ix=" << I.x << endl;
    //cout << "Iy=" << I.y << endl;
    // Verify
    Integer r1("615238761874518718411432514351734513434133154434"); // Thay bang r + 2 do o tren Q + H la cong them 2g vao rG
    r1 %= p;
    //cout << "secret value=" << r1 << endl;
    // Public key de chia se moi nguoi
    ECP::Point I1 = curve384.GetCurve().ScalarMultiply(G, r1); // r1.G;
    cout << "Public key to share" << endl;
    cout << "\tI1x=" << I1.x << endl;
    cout << "\tI1y=" << I1.y << endl;
    cout << curve384.GetCurve().Equal(I, I1) << endl;
    // Thay bang gia tri nhan duoc
    Integer Tx("34060460977502629051592487538187552405400197112805380678072861813882817025373123396467909619163377850738675890241189");
    Integer Ty("32768261101485250383294338930109981295588775928737849439894414893449847520720315993182657997665262918463262225380514");
    ECP::Point T(Tx, Ty);
    cout << "Public key received" << endl;
    cout << "\tTx=" << T.x << endl;
    cout << "\tTy=" << T.y << endl;
    ECP::Point K = curve384.GetCurve().ScalarMultiply(T, r1);
    cout << "Share key" << endl;
    cout << "\tKx=" << K.x << endl;
    cout << "\tKy=" << K.y << endl;   
}
