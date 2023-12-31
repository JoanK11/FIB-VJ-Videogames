using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class BalaPlayer : BalaBase {
    // Start is called before the first frame update
    GameObject prefab;
    public void init() {
        rotationSpeed = -10.0f;
        damage = 10.0f;
        base.initBala();
        Debug.Log(rotacionInicial);
        prefab = AssetDatabase.LoadAssetAtPath<GameObject>("Assets/Prefabs/SmallExplosion.prefab");
    }

    // Update is called once per frame
    void FixedUpdate() {

        Move();
    }
    protected void OnTriggerEnter(Collider other) {
        Debug.Log(other.gameObject.name + " ha entrado en el colider de " + gameObject.name);
        

        if (other.gameObject.tag == "Enemy") {
            EnemyBase enemy = other.gameObject.GetComponent<EnemyBase>();
            enemy.takeDamage(GetDamage());
        }
        GameObject explosion = Instantiate(prefab, transform.position, Quaternion.identity);
        Destroy(explosion, 1.0f);
        Destroy(gameObject);
    }
    
}