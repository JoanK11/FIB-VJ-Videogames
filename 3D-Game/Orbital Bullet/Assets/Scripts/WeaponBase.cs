using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class WeaponBase : MonoBehaviour
{
    // Start is called before the first frame update
    public abstract void Shoot(Vector3 pos, Quaternion initialRotation, Transform parent, float orientation);
}
