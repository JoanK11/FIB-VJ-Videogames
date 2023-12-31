using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
public class BalaEnemy : BalaBase
{
    // Start is called before the first frame update
    GameObject prefab;
    public void init() {
        rotationSpeed = -70.0f;
        damage = 10.0f;
        initBala();
        prefab = AssetDatabase.LoadAssetAtPath<GameObject>("Assets/Prefabs/SmallExplosion.prefab");
    }

    // Update is called once per frame
    void FixedUpdate()
    {

        Move();
    }
    protected void OnTriggerEnter(Collider other)
    {
        Debug.Log(other.gameObject.name + " ha entrado en el colider de " + gameObject.name);
        MeshRenderer mesh = GetComponent<MeshRenderer>();
        mesh.enabled = false;

        if (other.gameObject.tag == "Player")
        {
            MovePlayer player = other.gameObject.GetComponent<MovePlayer>();
            //player.takeDamage(GetDamage());
        }
        GameObject explosion = Instantiate(prefab, transform.position, Quaternion.identity);
        Destroy(explosion, 1.0f);
        Destroy(gameObject);
    }
}
